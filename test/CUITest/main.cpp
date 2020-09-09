#include "sobj.h"
#include "sapp/scuiapp.h"
class TestApp : public slib::sapp::SCuiApp {
public:
	TestApp() : slib::sapp::SCuiApp({
			kv("app", V({
					kv("name", "Test"),
					//kv("type", slib::sapp::SCUI_APP | slib::sapp::SINGLE_COMMAND),
					kv("type", slib::sapp::SCUI_APP | slib::sapp::MULTI_COMMAND),
					kv("creator", "Yuji Suehiro"),
					kv("version", "1.0.0"),
					kv("develop", "2012/12/5"),
					kv("license", "MIT license.")
				})
			),
			//kv("command", V({ kv("_exec", V({ kv("require", V({ "beta", "_args" })) })) })),
			kv("command", V({ 
				kv("read", V({ kv("require", V({ "beta" })), kv("option", V({"alpha"})), kv("description", "Read command.") })),
				kv("write", V({ kv("require", V({ "_args" })), kv("option", V({"gamma"})), kv("description", "Write command.") }))
			})),
			kv("option",
				V({
					kv("alpha", V({ kv("short", "a"), kv("type", "bool"), kv("description", "Set alpha mode.") })),
					kv("beta", V({ kv("short", "b"), kv("caption", "text"), kv("description", "Set param string.") })),
					kv("gamma", V({ kv("short", "c"), kv("type", "num"), kv("default", 3.14), kv("caption", "integer"), kv("description", "Set param value.") })),
					kv("_args", V({ kv("description", "argments...") }))
				})
			)
		}
	) {}
	~TestApp() {}
	int exec() {
		if (preference["_cmd"] == "_exec") {
			std::cout << "Exec app." << NEW_LINE << preference["beta"] << std::endl;
			if (preference["alpha"]) std::cout << preference["gamma"] << std::endl;
		}
		else if (preference["_cmd"] == "read") {
			std::cout << "Exec app read." << NEW_LINE << preference["beta"] << std::endl;
			if (preference["alpha"]) std::cout << preference["gamma"] << std::endl;
		}
		else if (preference["_cmd"] == "write") {
			std::cout << "Exec app write." << NEW_LINE << preference["_args"] << std::endl;
		}
		return 0;
	}
};
int main(int argc, const char **argv) {
	TestApp app;
	app.run(argc, argv);
	return 0;
}