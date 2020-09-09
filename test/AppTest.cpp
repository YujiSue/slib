#include "Test.h"

class TestApp : public slib::sapp::SCuiApp {
public:
        TestApp() : slib::sapp::SCuiApp({
                        kv("app", V({
                                        kv("name", "Test"),
                                        kv("type", slib::sapp::SCUI_APP | slib::sapp::SINGLE_COMMAND),
                                        kv("creator", "Yuji Suehiro"),
                                        kv("version", "1.0.0"),
                                        kv("develop", "2012/12/5"),
                                        kv("license", "MIT license.")
                                })
                        ),
                        kv("command", V({ kv("_exec", V({ kv("require", V({ "alpha", "_args" })) })) })),
                        kv("option",
                                V({
                                        kv("alpha", V({ kv("short", "a"), kv("type", "bool"), kv("description", "Set alpha mode.") })),
                                        kv("beta", V({ kv("short", "b"), kv("caption", "text"), kv("description", "Set param string.") })),
                                        kv("gamma", V({ kv("short", "c"), kv("type", "num"), kv("caption", "integer"), kv("description", "Set param value.") })),
                                        kv("_args", V({ kv("description", "argments...") }))
                                })
                        )
                }
        ) {}
        ~TestApp() {}
        int exec() {
                std::cout << "Exec app." << std::endl;
                return 0;
        }
};
void test::SCUIAppTest() {
        TestApp app;    app.exec();
}
void test::SPlugInAppTest() {
        slib:: sapp::SPlugIn<int, double, slib::String> plugin("PlugInTest.dll", "dlltest");
        std::cout << plugin.exec(10, 3.1415926, "12345") << std::endl;
}
void test::SNodeJSAppTest() {
        /*
        slib::SSystem::exec("npm start");
        slib::SSystem::exec("start http://localhost:3000");
        */
}