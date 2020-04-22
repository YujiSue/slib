#ifndef SLIB_SNODE_APP
#define SLIB_SNODE_APP

#include "sobj.h"
#include "node/node_api.h"

namespace slib {
	namespace sapp {
		#define DECLARE_NAPI_METHOD(name, func) { name, 0, func, 0, 0, 0, napi_default, 0 }

		class SNodeUtil {
		public:
			typedef enum {
				JS_NULL = 0,
				JS_NUMBER = 1,
				JS_BOOL = 2,
				JS_STRING = 3,
				JS_ARRAY = 4,
				JS_DICT = 5,
			} JS_OBJ_TYPE;

			static bool nodeInstalled();
			static bool gypInstalled();

			//static void makeMain(const char *path, const char *cname);
			//static void makeGypBind(const char *path, sobj profile);
			
			static JS_OBJ_TYPE jsType(napi_env env, napi_value val);
			
			static size_t jsStrlen(napi_env env, napi_value val);
			static napi_value jsnull(napi_env env);
			static napi_value jsbool(napi_env env, bool b);
			static napi_value jsint(napi_env env, int i);
			static napi_value jsuint(napi_env env, unsigned int ui);
			static napi_value jslong(napi_env env, sint i);
			static napi_value jsreal(napi_env env, double d);
			static napi_value jsnum(napi_env env, SNumber& n);
			static napi_value jsstr(napi_env env, const char* s);
			static napi_value jsarray(napi_env env, SArray& array);
			static napi_value jsdict(napi_env env, SDictionary& dict);
			static napi_value jsobj(napi_env env, sobj obj);
			static bool toBool(napi_env env, napi_value val);
			static int toInt(napi_env env, napi_value val);
			static unsigned int toUint(napi_env env, napi_value val);
			static sinteger toLong(napi_env env, napi_value val);
			static sreal toReal(napi_env env, napi_value val);
			static SNumber toNum(napi_env env, napi_value val);
			static String toStr(napi_env env, napi_value val);
			static SArray toArray(napi_env env, napi_value val);
			static SDictionary toDict(napi_env env, napi_value val);
			static sobj toObj(napi_env env, napi_value val);
			template<class Cls>
			static Cls* toClass(napi_env env, napi_callback_info info, size_t* argc, napi_value* args) {
				napi_status st;
				napi_value instance;
				Cls* c;
				st = napi_get_cb_info(env, info, argc, args, &instance, nullptr);
				if (st != napi_ok) napi_throw_error(env, NULL, "");
				st = napi_unwrap(env, instance, reinterpret_cast<void**>(&c));
				if (st != napi_ok) napi_throw_error(env, NULL, "");
				return c;
			}
		};
		
		template<class Wrapper>
		class SNodeApp {
		private:
			napi_env _env;
			napi_ref _wrapper;
			static napi_ref _constructor;
			
		public:
			static napi_value Init(napi_env env, napi_value exports);
			static void Destructor(napi_env env, void *object, void *finalize);
			/*
			Require 2 additional functions in child class.
			static String className();
			static Array<napi_property_descriptor> descriptor();
			 */
        
		protected:
			explicit SNodeApp();
			virtual ~SNodeApp();
        
			static napi_value New(napi_env env, napi_callback_info info);
		};
		
		template<class Wrapper>
		napi_ref SNodeApp<Wrapper>::_constructor;
		template<class Wrapper>
		SNodeApp<Wrapper>::SNodeApp() : _env(nullptr), _wrapper(nullptr) {}
		template<class Wrapper>
		SNodeApp<Wrapper>::~SNodeApp() { napi_delete_reference(_env, _wrapper); }
		template<class Wrapper>
		napi_value SNodeApp<Wrapper>::Init(napi_env env, napi_value exports) {
			napi_status status;
			napi_value result;
			Array<napi_property_descriptor> desc = Wrapper::descriptor();
			status = napi_define_class(env, Wrapper::className(), NAPI_AUTO_LENGTH, New, nullptr, desc.size(), desc.ptr(), &result);
			if (status != napi_ok) napi_throw_error(env, NULL, "");
			status = napi_create_reference(env, result, 1, &_constructor);
			if (status != napi_ok) napi_throw_error(env, NULL, "");
			status = napi_set_named_property(env, exports, Wrapper::className(), result);
			if (status != napi_ok) napi_throw_error(env, NULL, "");
			return exports;
		}
		template<class Wrapper>
		void SNodeApp<Wrapper>::Destructor(napi_env env, void* object, void* finalize) {
			reinterpret_cast<Wrapper*>(object)->~Wrapper();
		}
		template<class Wrapper>
		napi_value SNodeApp<Wrapper>::New(napi_env env, napi_callback_info info) {
			napi_status status;
			napi_value target;
			status = napi_get_new_target(env, info, &target);
			if (status != napi_ok) napi_throw_error(env, NULL, "");
			bool is_constructor = target != nullptr;
			if (is_constructor) {
				size_t argc = 1;
				napi_value args[1];
				napi_value jsthis;
				status = napi_get_cb_info(env, info, &argc, args, &jsthis, nullptr);
				if (status != napi_ok) napi_throw_error(env, NULL, "");
				Wrapper *instance = new Wrapper();
				instance->_env = env;
				status = napi_wrap(env,
                           jsthis,
                           reinterpret_cast<void*>(instance),
                           Wrapper::Destructor,
                           nullptr,  // finalize_hint
                           &instance->_wrapper
				);
				return jsthis;
			} else {
				size_t argc_ = 1;
				napi_value args[1];
				status = napi_get_cb_info(env, info, &argc_, args, nullptr, nullptr);
				if (status != napi_ok) napi_throw_error(env, NULL, "");
				const size_t argc = 1;
				napi_value argv[argc] = {args[0]};
				if (status != napi_ok) napi_throw_error(env, NULL, "");
				napi_value result;
				status = napi_get_reference_value(env, _constructor, &result);
				if (status != napi_ok) napi_throw_error(env, NULL, "");
				napi_value instance;
				status = napi_new_instance(env, result, argc, argv, &instance);
				if (status != napi_ok) napi_throw_error(env, NULL, "");
				return instance;
			}
		}
	}
}

#endif