#ifndef SSCI_CLUSTER_H
#define SSCI_CLUSTER_H

#include "smath/smath.h"
#include "sbasic/ptr.h"
#include "sbasic/string.h"

using namespace slib;
using namespace slib::smath;

namespace slib {
	namespace ssci {
		template<typename T>
		extern inline double EuclidDistance(T& vec1, T& vec2) {
			return sgeom::distance(vec1, vec2);
		}
		template<typename T>
		extern inline double HammingDistance(T& vec1, T& vec2) {
			size_t count = 0;
			sforeach2(vec1, vec2) {
				if (E1_ != E2_) ++count;
			}
			return count;
		}
		template<typename T>
		extern inline double LevenshteinDistance(T& vec1, T& vec2) {
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
					dist[r][c] = tmp[sstat::argmin(tmp, 3)];
				}
			}
			return dist.last();
		}
		template<typename T>
		extern inline double DifferentialDistance(T& vec1, T& vec2) {
			double dist = 0.0;
			sforeach2(vec1, vec2) { dist += abs(E1_ - E2_); }
			return dist;
		}
		template<typename T>
		extern inline double Differential2dDistance(T& vec1, T& vec2) {
			double dist = 0.0;
			sforeach2(vec1, vec2) {
				auto v = E1_ - E2_;
				sforeach(v) dist += abs(E_) / 2.0;
			}
			return dist;
		}


		typedef enum {
			SINGLE_LINK = 1,
			COMPLETE_LINK = 2,
			GROUP_AVERAGE = 3,
			WARD = 4,
		} HCLUSTER_METHOD;
		struct scluster;
		extern inline void printHC(stringarray& strs, srange range, ssci::scluster* node);
		struct scluster {
			sint index, layer;
			scluster* parent, *elements[2];
			double distance;

			scluster() : index(-1), layer(0), parent(nullptr), distance(0.0) { memset(elements, 0, sizeof(scluster*) * 2); }
			scluster(sint i) : scluster() {
				index = i; elements[0] = this;
			}
			scluster(scluster* c1, scluster *c2, double d) : scluster() {
				c1->parent = this; c2->parent = this;
				c1->increment(); c2->increment();
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
			void increment() {
				++layer; 
				if (elements[0] != this) elements[0]->increment(); 
				if (elements[1]) elements[1]->increment();
			}
			void toStrs(stringarray &array) {
				srange range(0, array.size());
				printHC(array, range, this);
			}
		};
		void printHC(stringarray& strs, srange range, ssci::scluster* node) {
			if (node->isOrigin()) strs[range.begin] << SPACE * (node->layer * 2) << "- #" << (node->index + 1);
			else {
				auto count = node->elements[0]->count();
				strs[range.begin + 2 * count - 1] << SPACE * (node->layer * 2) << "-|";
				printHC(strs, srange(range.begin, range.begin + 2 * (count - 1)), node->elements[0]);
				printHC(strs, srange(range.begin + 2 * count, range.end), node->elements[1]);
				auto r = range.begin + 2 * count - 2;
				while (strs[r][node->layer * 2 + 2] != '-') {
					strs[r][node->layer * 2 + 1] = '|'; --r;
				}
				r = range.begin + 2 * count;
				while (strs[r][node->layer * 2 + 2] != '-') {
					strs[r][node->layer * 2 + 1] = '|'; ++r;
				}
			}
		}
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
		extern inline void LWUpdate(sint c1, sint c2, Array<scluster> &clusters, Array<scluster *> &tree, svecd &dist, svecd &newdist, HCLUSTER_METHOD method) {
			double a[4];
			double d = dist[distIndex(c1, c2, tree.size())];
			auto size = tree.size();
			if (1 < size) {
				switch (method)
				{
				case SINGLE_LINK:
					a[0] = 0.5; a[1] = 0.5; a[2] = 0.0; a[3] = -0.5;
					distUpdate(a, d, c1, c2, tree.size(), dist, newdist.ptr());
					break;
				case COMPLETE_LINK:
					a[0] = 0.5; a[1] = 0.5; a[2] = 0.0; a[3] = 0.5;
					distUpdate(a, d, c1, c2, tree.size(), dist, newdist.ptr());
					break;
				case GROUP_AVERAGE:
				{
					auto n1 = tree[c1]->count(), n2 = tree[c2]->count();
					a[0] = (double)n1 / (n1 + n2); a[1] = (double)n2 / (n1 + n2); a[2] = 0.0; a[3] = 0.0;
					distUpdate(a, d, c1, c2, tree.size(), dist, newdist.ptr());
					break;
				}
				case WARD:
				{
					auto ptr = newdist.ptr();
					auto n1 = tree[c1]->count(), n2 = tree[c2]->count();
					sforin(i, 0, size) {
						if (i == c1 || i == c2) continue;
						sforin(j, i + 1, size) {
							if (j == c1 || j == c2) continue;
							*ptr = dist[distIndex(i, j, size)];
							++ptr;
						}
						auto n3 = tree[i]->count();
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
			
			clusters.add(scluster(tree[c1], tree[c2], d));
			tree.add(&clusters.last());
			tree.removeAt(c2);
			tree.removeAt(c1);
		}
		template<typename T>
		extern inline void hcluster(svec<T>& data, Array<scluster>& clusters, Array<scluster*>& tree, HCLUSTER_METHOD method = WARD,
			SFunction<double(T&, T&)> distance = EuclidDistance<T>) {
			svecd dist(data.size() * (data.size() - 1) / 2, 0.0), newdist;
			sint integrate[2];
			auto dp = dist.ptr();
			sforin(i, 0, data.size()-1) {
				clusters.add(scluster(i));
				tree.add(&clusters.last());
				sforin(j, i + 1, data.size()) {
					*dp = distance(data[i], data[j]); ++dp;
				}
			}
			clusters.add(scluster(data.size() - 1));
			tree.add(&clusters.last());
			while (1 < tree.size()) {
				newdist.resize((tree.size() - 1)* (tree.size() - 2) / 2);
				clusterIndex(sstat::argmin(dist), tree.size(), integrate);
				LWUpdate(integrate[0], integrate[1], clusters, tree, dist, newdist, method);
			}
		}
		template<typename T>
		class HClusterAnalysis {
		public:
			svec<T>* data;
			HCLUSTER_METHOD method;
			SFunction<double(T&, T&)> distFunc;
			Array<scluster> clusters;
			Array<scluster *> tree;

		public:
			HClusterAnalysis(svec<T>& dat) {
				data = &dat; clusters.reserve(dat.size() * 2);
			}
			~HClusterAnalysis() {}

			void analyze(HCLUSTER_METHOD m = WARD,
				SFunction<double(T&, T&)> distance = EuclidDistance<T>) {
				method = m;
				distFunc = distance;
				hcluster<T>(*data, clusters, tree, method, distFunc);
			}
			//void plot();
			String summary(intarray& array) {
				String str;
				array.reserve(data->size());
				auto beg = clusters.ptr(data->size());
				auto it = sarr_iter<scluster>(beg);
				while (it < clusters.end()) {
					if (E_.elements[0]->isOrigin()) {
						str << "#" << (E_.elements[0]->index + 1) << TAB;
						array.add(E_.elements[0]->index);
					}
					else str << "%" << (E_.elements[0] - beg + 1) << TAB;
					if (E_.elements[1]->isOrigin()) {
						str << "#" << (E_.elements[1]->index + 1) << TAB;
						array.add(E_.elements[1]->index);
					}
					else str << "%" << (E_.elements[1] - beg + 1) << TAB;
					str << E_.distance << NEW_LINE;
					NEXT_;
				}
				return str;
			}
			void order(intarray& array) {
				array.reserve(data->size());
				auto it = clusters.begin() + data->size();
				while (it < clusters.end()) {
					if (E_.elements[0]->isOrigin()) array.add(E_.elements[0]->index);
					if (E_.elements[1]->isOrigin()) array.add(E_.elements[1]->index);
					NEXT_;
				}
			}
			void structure(stringarray& array) {
				array.resize(data->size() * 2);
				tree[0]->toStrs(array);
			}
			/*
			void export(const char* path) {
				intarray sorted;
				sorted.reserve(data->size());
				stringarray strs(data->size() * 2);
				sio::SFile clusters(String(path) + ".cluster", sio::WRITE | sio::CREATE),
					sorted(String(path) + ".sorted", sio::WRITE | sio::CREATE),
					tree(String(path) + ".tree", sio::WRITE | sio::CREATE);
				



			}
			*/
		};

		template<typename T>
		extern inline void MassCentroid(svec<T>& data, sveci& group, svec<T>& centroid, svecb &buffer) {
			if (data.empty()) return;
			buffer.resize(centroid.size() * 4);
			buffer.reset(0);
			auto countptr = reinterpret_cast<sint*>(buffer.ptr());
			sforeach(centroid) E_.reset(0);
			sforeach2(data, group) {
				centroid[E2_] += E1_; ++(countptr[E2_]);
			}
			sforeach(centroid) { E_ /= (*countptr); ++countptr; }
		}
		template<typename T>
		extern inline void ModeCentroid(svec<T>& data, sveci& group, svec<T>& centroid, svecb& buffer) {
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
		template<typename T>
		extern inline void MedianCentroid(svec<T>& data, sveci& group, svec<T>& centroid, svecb& buffer) {
			if (data.empty()) return;
			smat<std::pair<Array<T*>, sint>> index(centroid.size(), centroid[0].size());
			sforeach(index) {
				E_.first.reserve(data.size());
				E_.second = 0;
			}
			sforeach2(data, group) {
				
			}


			
		}
		template<typename T>
		extern inline void FrequencyCentroid(svec<T>& data, sveci& group, svec<T>& centroid, svecb& buffer) {
			if (data.empty()) return;
			auto rank = data[0][0].size();
			sforeach(centroid) {
				E_.resize(data[0].size());
				sforeach_(cit, E_) { cit->resize(rank); cit->reset(0); }
			}
			sforeach2(data, group) { centroid[E2_] += E1_; }
			sforeach(centroid) {
				sforeach_(cit, E_) {
					auto sum = sstat::sum(*cit);
					(*cit) /= sum;
				}
			}
		}

		template<typename T>
		extern inline void initCentroid(int cluster, svec<T>& data, svec<T>& centroid) {
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
		template<typename T>
		extern inline void initCentroidPlus(int cluster, svec<T>& data, svec<T>& centroid, SFunction<double(T&, T&)>& distance) {
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
		
		template<typename T>
		extern inline void kmeans(int cluster, svec<T>& data, sveci& group, svec<T>& centroid, svecb &buffer,
			int iter, const char* method = "default",
			SFunction<double(T&, T&)> distance = EuclidDistance<T>,
			SFunction<void(svec<T>&, sveci&, svec<T>&, svecb &)> center = MassCentroid<T>) {
			if (data.empty()) return;
			auto row = data.size(), col = data[0].size();
			svecd dist(cluster, 0);
			group.resize(row, 0);
			centroid.resize(cluster);
			svec<T> tmpcent(cluster);
			SRandom rand;
			sforin(c, 0, cluster) {
				centroid[c].resize(col, 0);
				tmpcent[c].resize(col, 0);
			}
			String alg = method;
			if (alg == "kmeans++") initCentroidPlus<T>(cluster, data, centroid, distance);
			else initCentroid<T>(cluster, data, centroid);
			sforin(i, 0, iter) {
				auto rptr = data.ptr();
				auto gp = group.ptr();
				sforin(r, 0, row) {
					sforin(c, 0, cluster) dist[c] = distance(*rptr, centroid[c]);
					*gp = sstat::argmin(dist);
					++rptr; ++gp;
				}
				center(data, group, tmpcent, buffer);
				if (centroid == tmpcent) break;
				else centroid.swap(tmpcent);
			}
		}

		template<typename T>
		class KMeansAnalysis {
		public:
			sint cluster, iter;
			svecb buffer;
			sveci group;
			svec<T> *data, centroid;
			SFunction<double(T&, T&)> distFunc;
			SFunction<void(svec<T>&, sveci&, svec<T>&, svecb&)> centerFunc;
			SWork* threads;

		public:
			KMeansAnalysis(svec<T> &dat) {
				data = &dat;
				group.resize(data->size(), 0);
			}
			~KMeansAnalysis() {}

			void setMultiThreadMode(SWork &w) { threads = &w; }
			void analyze(sint n = 8, const char *method = "default",
				SFunction<double(T&, T&)> dist = EuclidDistance<T>,
				SFunction<void(svec<T>&, sveci&, svec<T>&, svecb&)> center = MassCentroid<T>, sint i = 300) {
				try {
					cluster = n; iter = i;
					distFunc = dist;
					centerFunc = center;
					centroid.resize(cluster);
					sforin(c, 0, cluster) centroid[c].resize(data->first().size());
					kmeans<T>(cluster, *data, group, centroid, buffer, iter, method, distFunc, centerFunc);
				}
				catch (SException ex) {
					ex.print();
				}
			}
			void print() {
				sforin(i, 0, cluster) {
					std::cout << "#" << (i + 1) << std::endl;
					sforeach2(group, *data) {
						if (E1_ == i) std::cout << TAB << it2.second - data->begin() << std::endl;
					}
				}

			}
			//void plot();
			//void summary();
			//void export();
		};

	}
}

#endif

