#ifndef SSCI_CLUSTER_H
#define SSCI_CLUSTER_H

#include "smath/smath.h"
#include "sbasic/ptr.h"
#include "sbasic/string.h"

using namespace slib;
using namespace slib::smath;

namespace slib {
	namespace ssci {
		template<typename T, class M>
		extern inline double EuclidDistance(svec<T, M>& vec1, svec<T, M>& vec2) {
			return sgeom::distance(vec1, vec2);
		}
		template<typename T, class M>
		extern inline double HammingDistance(svec<T, M>& vec1, svec<T, M>& vec2) {
			size_t count = 0;
			sforeach2(vec1, vec2) {
				if (E1_ != E2_) ++count;
			}
			return count;
		}
		template<typename T, class M>
		extern inline double LevenshteinDistance(svec<T, M>& vec1, svec<T, M>& vec2) {
			smati dist(vec1.size() + 1, vec2.size() + 1, 0);
			sint cost, tmp[3];
			sforin(r, 0, dist.row) dist[r][0] = r;
			sforin(c, 0, dist.col) dist[0][c] = c;
			sforin(r, 1, dist.row) {
				sforin(c, 1, dist.col) {
					cost = vec1[r - 1] == vec2[c - 1] ? 0 : 1;
					tmp[0] = dist[r - 1][c] + 1;
					tmp[1] = dist[r][c - 1] + 1;
					tmp[2] = dist[r - 1][c - 1] + cost;
					dist[r][c] = sstat::argmin(tmp, 3);
				}
			}
			return dist.last();
		}

		typedef enum {
			SINGLE_LINK = 1,
			COMPLETE_LINK = 2,
			GROUP_AVERAGE = 3,
			WARD = 4,
		} HCLUSTER_METHOD;

		struct scluster {
			sint index;
			scluster* parent, *elements[2];
			double distance;

			scluster() : index(-1), parent(nullptr), distance(0.0) { memset(elements, 0, sizeof(scluster*) * 2); }
			scluster(sint i) : scluster() {
				index = i; elements[0] = this;
			}
			scluster(scluster* c1, scluster *c2, double d) : scluster() {
				c1->parent = this; c2->parent = this;
				elements[0] = c1; elements[1] = c2; distance = d;
			}
			~scluster() {}
			scluster* sibring() { return parent->elements[0] == this ? parent->elements[1] : parent->elements[0]; }
			
			bool isOrigin() const { return elements[1] == nullptr; }
			sint count() {
				if (isOrigin()) return 1;
				auto c = 0;
				c += elements[0]->count();
				c += elements[1]->count();
				return c;
			}
		};
		using sclusterp = Pointer<scluster>;

		inline void clusterIndex(sint i, sint n, sint *c) {
			sforin(r, 0, n) {
				if (n - r - 1 <= i) i -= (n - r - 1);
				else {
					c[0] = r;
					c[1] = i + r + 1;
					break;
				}
			}
		}
		inline sint distIndex(sint i1, sint i2, sint n) {
			return  (2 * n - i1 - 1) * i1 / 2 + (i2 - i1 - 1);
		}
		inline double newDist(double* a, double d, double d1, double d2) {
			return a[0] * d1 + a[1] * d2 + a[2] * d + a[3] * abs(d1 - d2);
		}
		inline void distUpdate(double *a, double &d, sint c1, sint c2, size_t size, svecd& dist, double *ptr) {
			sforin(i, 0, size) {
				if (i == c1 || i == c2) continue;
				sforin(j, i + 1, size) {
					if (j == c1 || j == c2) continue;
					*ptr = dist[distIndex(i, j, size)];
					++ptr;
				}
				*ptr = newDist(a, d,
					dist[distIndex(i < c1 ? i : c1, i < c1 ? c1 : i, size)],
					dist[distIndex(i < c2 ? i : c2, i < c2 ? c2 : i, size)]);
				++ptr;
			}
		}
		template<typename T, class M>
		extern inline void LWUpdate(sint c1, sint c2, Array<sclusterp> &root, svecd &dist, svecd &newdist, HCLUSTER_METHOD method) {
			double a[4];
			double d = dist[distIndex(c1, c2, root.size())];
			auto size = root.size();
			if (1 < size) {
				switch (method)
				{
				case SINGLE_LINK:
					a[0] = 0.5; a[1] = 0.5; a[2] = 0.0; a[3] = -0.5;
					distUpdate(a, d, c1, c2, root.size(), dist, newdist.ptr());
					break;
				case COMPLETE_LINK:
					a[0] = 0.5; a[1] = 0.5; a[2] = 0.0; a[3] = 0.5;
					distUpdate(a, d, c1, c2, root.size(), dist, newdist.ptr());
					break;
				case GROUP_AVERAGE:
				{
					auto n1 = root[c1]->count(), n2 = root[c2]->count();
					a[0] = (double)n1 / (n1 + n2); a[1] = (double)n2 / (n1 + n2); a[2] = 0.0; a[3] = 0.0;
					distUpdate(a, d, c1, c2, root.size(), dist, newdist.ptr());
					break;
				}
				case WARD:
				{
					auto ptr = newdist.ptr();
					auto n1 = root[c1]->count(), n2 = root[c2]->count();
					sforin(i, 0, size) {
						if (i == c1 || i == c2) continue;
						sforin(j, i + 1, size) {
							if (j == c1 || j == c2) continue;
							*ptr = dist[distIndex(i, j, size)];
							++ptr;
						}
						auto n3 = root[i]->count();
						a[0] = (double)(n1 + n3) / (n1 + n2 + n3);
						a[1] = (double)(n2 + n3) / (n1 + n2 + n3);
						a[2] = (double)(-n3) / (n1 + n2 + n3);
						a[3] = 0.0;
						*ptr = newDist(a, d,
							dist[distIndex(i < c1 ? i : c1, i < c1 ? c1 : i, size)],
							dist[distIndex(i < c2 ? i : c2, i < c2 ? c2 : i, size)]);
						++ptr;
					}
					break;
				}
				default:
					break;
				}
				dist.swap(newdist);
			}
			scluster* sc = new scluster(root[c1], root[c2], d);
			root.add(sc);
			root.removeAt(c2);
			root.removeAt(c1);
		}
		
		template<typename T, class M>
		extern inline void hcluster(svec<svec<T, M>>& data, Array<sclusterp> &clusters, HCLUSTER_METHOD method = WARD,
			std::function<double(svec<T, M>&, svec<T, M>&)> distance = EuclidDistance<T, M>) {
			svecd dist(data.size() * (data.size() - 1) / 2, 0.0), newdist;
			sint integrate[2];
			auto dp = dist.ptr();
			sforin(i, 0, data.size()-1) {
				clusters.add(i);
				sforin(j, i + 1, data.size()) {
					*dp = distance(data[i], data[j]); ++dp;
				}
			}
			clusters.add(new scluster(data.size() - 1));
			while (1 < clusters.size()) {
				newdist.resize((clusters.size() - 1)* (clusters.size() - 2) / 2);
				clusterIndex(sstat::argmin(dist), clusters.size(), integrate);
				LWUpdate<T, M>(integrate[0], integrate[1], clusters, dist, newdist, method);
			}
		}

		template<typename T, class M>
		class HClusterAnalysis {
		public:
			svec<svec<T, M>>* data;
			HCLUSTER_METHOD method;
			std::function<double(svec<T, M>&, svec<T, M>&)> distFunc;
			Array<sclusterp> clusters;

		public:
			HClusterAnalysis(svec<svec<T, M>>& dat) {
				data = &dat;
			}
			~HClusterAnalysis() {}

			void analyze(HCLUSTER_METHOD m = WARD,
				std::function<double(svec<T, M>&, svec<T, M>&)> distance = EuclidDistance<T, M>) {
				method = m;
				distFunc = distance;
				hcluster(*data, clusters, method, distFunc);
			}
			//void plot();
			//void summary();
			//void export();
		};

		template<typename T, class M>
		extern inline void massCentroid(svec<svec<T, M>>& data, sveci& group, svec<svec<T, M>>& centroid) {
			if (data.empty()) return;
			sveci count(centroid.size());
			sforeach(centroid) E_.reset(0);
			sforeach2(data, group) {
				centroid[E2_] += E1_;
				++count[E2_];
			}
			sforeach2(centroid, count) E1_ /= E2_;
		}
		template<typename T, class M>
		extern inline void maxFreqCentroid(svec<svec<T, M>>& data, sveci& group, svec<svec<T, M>>& centroid) {
			if (data.empty()) return;
			sint cluster = centroid.size(), dim = centroid[0].size();
			smat<Map<T, sint>> count(centroid.size(), dim);
			T maxkey; sint max;
			sforeach2(data, group) {
				sforin(i, 0, dim) {
					if (count[E2_][i].hasKey(E1_[i])) ++count[E2_][i][E1_[i]];
					else count[E2_][i][E1_[i]] = 0;
				}
			}
			sforin(i, 0, cluster) {
				sforin(j, 0, dim) {
					maxkey = count[i][j].begin().key;
					max = count[i][j].begin().value;
					sforeach(count[i][j]) {
						if (max < E_.value) {
							max = E_.value;
							maxkey = E_.key;
						}
					}
					centroid[i][j] = maxkey;
				}
			}
		}
		template<typename T, class M>
		extern inline void initCentroid(int cluster, svec<svec<T, M>>& data, svec<svec<T, M>>& centroid) {
			if (data.size() < cluster) return;
			intarray extract;
			SRandom rand;
			while (extract.size() < cluster) {
				auto i = rand.iruni(0, data.size());
				if (extract.find(i) == NOT_FOUND) extract.add(i);
			}
			sforin(c, 0, cluster) {
				centroid[c] = data[extract[c]];
			}
		}
		template<typename T, class M>
		extern inline void initCentroidPlus(int cluster, svec<svec<T, M>>& data, svec<svec<T, M>>& centroid, std::function<double(svec<T, M>&, svec<T, M>&)>& distance) {
			if (data.size() < cluster) return;
			intarray extract;
			auto row = data.size(), col = data[0].size();
			svecd dist(row);
			SRandom rand;
			extract.add(rand.iruni(0, row));
			while (extract.size() < cluster) {
				auto rptr = data.ptr();
				auto& prev = data[extract.last()];
				auto d = dist.ptr();
				sforin(r, 0, row) {
					if (extract.find(r) == NOT_FOUND) (*d) = distance(*rptr, prev);
					else (*d) = 0;
					++rptr; ++d;
				}
				dist /= sstat::sum(dist);
				auto prob = rand.runi();
				sforeach(dist) {
					prob -= E_;
					if (prob <= 0.0) {
						extract.add(INDEX_(dist));
						break;
					}
				}
			}
			sforin(c, 0, cluster) centroid[c] = data[extract[c]];
		}
		
		template<typename T, class M>
		extern inline void kmeans(int cluster, svec<svec<T, M>>& data, sveci& group, svec<svec<T, M>>& centroid,
			int iter, const char* method = "default",
			std::function<double(svec<T, M>&, svec<T, M>&)> distance = EuclidDistance<T, M>,
			std::function<void(svec<svec<T, M>>&, sveci&, svec<svec<T, M>>&)> center = massCentroid<T, M>) {
			if (data.empty()) return;
			auto row = data.size(), col = data[0].size();
			svecd dist(cluster, 0);
			group.resize(row, 0);
			centroid.resize(cluster);
			svec<svec<T, M>> tmpcent(cluster);
			SRandom rand;
			sforin(c, 0, cluster) {
				centroid[c].resize(col, 0);
				tmpcent[c].resize(col, 0);
			}
			String alg = method;
			if (alg == "kmeans++") initCentroidPlus<T, M>(cluster, data, centroid, distance);
			else initCentroid<T, M>(cluster, data, centroid);
			sforin(i, 0, iter) {
				auto rptr = data.ptr();
				auto gp = group.ptr();
				sforin(r, 0, row) {
					sforin(c, 0, cluster) dist[c] = distance(*rptr, centroid[c]);
					*gp = sstat::argmin(dist);
					++rptr; ++gp;
				}
				center(data, group, tmpcent);
				if (centroid == tmpcent) break;
				else centroid.swap(tmpcent);
			}
		}

		template<typename T, class M>
		class KMeansAnalysis {
		public:
			sint cluster, iter;
			sveci group;
			svec<svec<T, M>> *data, centroid;
			std::function<double(svec<T, M>&, svec<T, M>&)> distFunc;
			std::function<void(int, svec<svec<T, M>>&, sveci&, svec<T, M>&)> centerFunc;
			SWork* threads;

		public:
			KMeansAnalysis(svec<svec<T, M>> &dat) {
				data = &dat;
				group.resize(data->size(), 0);
			}
			~KMeansAnalysis() {}

			void setMultiThreadMode(SWork &w) { threads = &w; }
			void analyze(sint n = 8, sint i = 300, const char *method = "default",
				std::function<double(svec<T, M>&, svec<T, M>&)> dist = EuclidDistance,
				std::function<void(int, svec<svec<T, M>>&, sveci&, svec<T, M>&)> center = massCentroid) {
				cluster = n; iter = i;
				distFunc = dist;
				centerFunc = center;
				centroid.resize(cluster);
				sforin(c, 0, cluster) centroid[c].resize(data->first().size());
				kmeans(cluster, *data, group, centroid, iter, method, distFunc, centerFunc);
			}
			//void plot();
			//void summary();
			//void export();
		};

	}
}

#endif

