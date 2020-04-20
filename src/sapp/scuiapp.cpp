#include "sapp/scuiapp.h"

using namespace slib;
using namespace slib::sapp;

inline void outputOp(sobj &option, sobj &list, bool op) {
	String sop, lop;
    sforeach(list) {
        if (E_.string().beginWith("_") || !option.hasKey(E_)) continue;
		if (option[E_]["type"] == "exec") continue;
		else if (option[E_]["type"] == "bool") {
			if (option[E_].hasKey("short")) sop += option[E_]["short"];
			else lop << (op ? " { --" : " --") << E_ << (op ? " }" : "");
		}
		else {
			if (option[E_].hasKey("short")) lop << (op ? " { -" : " -") << option[E_]["short"] << " ";
			else lop << (op ? " { --" : " --") << E_ << " ";
			lop << SItalicText((option[E_].hasKey("caption")? option[E_]["caption"]:"arg")) << (op ? " }" : "");
		}
    }
	if (!sop.empty()) {
		std::cout << (op ? " { " : " ") << "-" << sop << (op ? " }" : "");
		if (!lop.empty()) std::cout << lop;
	}
	else if (!lop.empty()) std::cout << lop;
}
inline void outputSelectOp(sobj &option, sobj &list) {
    std::cout<<" ╎ ";
    sforeach(list) {
		if (E_.string().beginWith("_") || !option.hasKey(E_)) continue;
		if (option[E_]["type"] == "exec") continue;
		if (option[E_].hasKey("short")) std::cout << "[ -" << option[E_]["short"];
		else std::cout << "[ --" << E_;
		if (option[E_]["type"] == "bool") std::cout << " ]";
		else std::cout << SItalicText((option[E_].hasKey("caption") ? option[E_]["caption"] : "arg")) << " ]";
        if (it < list.end()-1) std::cout<<" or ";
    }
    std::cout<<" ╎";
}
inline int showUsage(SDictionary &profile, const sobj &cmd) {
    std::cout<<"Usage:"<<std::endl;
    std::cout<<"  "<<profile["app"]["name"];
	sobj app_command;
	if (profile["app"]["type"] == 0) app_command = profile["command"]["_exec"];
	else {
		if (!cmd) { std::cout << " <command> { options }" << std::endl; return 0; }
		else {
			std::cout << " " << cmd << std::flush;
			app_command = profile["command"][cmd];
		}
	}
    auto app_option = profile["option"];
    auto required = app_command["require"];
	if (!required.empty()) outputOp(app_option, required, false);
	if (required.contain("_args")) 
		std::cout << SItalicText(app_option["_args"].hasKey("caption") ? app_option["_args"]["caption"] : "argments...");
	if (required.contain("_out"))  
		std::cout << " > " << SItalicText(app_option["_out"].hasKey("caption") ? app_option["_out"]["caption"] : "output");
	if (required.contain("_pipe"))
		std::cout << " | " << SItalicText(app_option["_pipe"].hasKey("caption") ? app_option["_pipe"]["caption"] : "commands...");
	auto select = app_command["select"];
	if (!select.empty()) outputSelectOp(app_option, select);
    auto opt = app_command["option"];
    if (!opt.empty()) outputOp(app_option, opt, true);
    if (opt.contain("_args"))
		std::cout << " { " << SItalicText(app_option["_args"].hasKey("caption") ? app_option["_args"]["caption"] : "argments...") << " }";
    if (opt.contain("_out")) 
		std::cout << " { > " << SItalicText(app_option["_out"].hasKey("caption") ? app_option["_out"]["caption"] : "output") << " }";
	if (opt.contain("_pipe"))
		std::cout << " { | " << SItalicText(app_option["_pipe"].hasKey("caption") ? app_option["_pipe"]["caption"] : "commands...") << " }";
	std::cout << std::endl; 
	return SAPP_EXECTED;
}
inline String opHead(const char *key, const sobj &option) {
	return String("--") << key << (option[key].hasKey("short") ? "/-" + option[key]["short"] : "");
}
inline int showHelp(SDictionary &profile, const sobj& cmd) {
	std::cout << String(60, '=') <<std::endl;

    std::cout<<profile["app"]["name"]<<" v"<<profile["app"]["version"]<<std::endl;
    std::cout<<"Copyright (c) "<<SDate(profile["app"]["develop"]).year<<" "<<
    profile["app"]["creator"]<<". All rights reserved. "<<NEW_LINE<<
    profile["app"]["license"]<<std::endl;

	std::cout << String(". ")*30 << std::endl;
	std::cout<<std::endl;
    showUsage(profile, cmd);

    std::cout << String(". ") * 30 << std::endl;
	std::cout << std::endl;

	auto app_command = profile["command"];
	auto app_option = profile["option"];
	stringarray opt_;
	sforeach(app_option) {
		if (E_.value()["type"] == "exec") opt_.add(E_.key());
	}

    if (profile["app"]["type"] == SCUI_MCMD_APP) {
		if (!cmd) {
			std::cout << "[Command]" << std::endl;
			auto cmds = app_command.keyset();
			cmds.sort();
			sforeach(cmds) {
				std::cout << "  " << E_.filled(16, ' ') << ": " << app_command[E_]["description"] << std::endl;
				auto prof_ = profile["command"][E_];
			}
		}
		else {
			std::cout << "[Option]" << std::endl;
			auto command = profile["command"][cmd];
			auto required = command["require"];
			auto select = command["select"];
			auto ops = command["option"];

			if (required && !required.empty()) {
				sforeach(required) {
					if (E_.string().beginWith("_") || app_option[E_]["type"] == "exec") continue;
					String op_ = opHead(E_, app_option);
					std::cout << SColorText("red", "*") << " " << op_.filled(20) << ": " << app_option[E_]["description"] << std::endl;
				}
			}

			if (ops && !ops.empty()) {
				sforeach(ops) {
					if (E_.string().beginWith("_") || app_option[E_]["type"] == "exec") continue;
					String op_ = opHead(E_, app_option);
					std::cout << "  " << op_.filled(20) << ": " << app_option[E_]["description"] << std::endl;
				}
			}

			if (select) {
				sforeach(select) {
					if (E_.string().beginWith("_") || app_option[E_]["type"] == "exec") continue;
					String op_ = opHead(E_, app_option);
					std::cout << SColorText("blue", "#") << " " << op_.filled(20) << ": " << app_option[E_]["description"] << std::endl;
				}
			}
		}
    }
    else {
		if (!app_option.empty()) {
            std::cout<<"[Option]"<<std::endl;
            auto command = profile["command"]["_exec"];
            auto required = command["require"];
            auto select = command["select"];
            
            auto ops = app_option.keyset();
            ops.sort();
            sforeach(ops) {
                if (E_.beginWith("_") || app_option[E_]["type"] == "exec") continue;
				String op_ = opHead(E_, app_option);
				if (required.contain(E_)) std::cout << " " << SColorText("red", "*");
				else if (select.contain(E_)) std::cout << " " << SColorText("blue", "#");
				else std::cout << "  ";
				std::cout << op_.filled(20) << ": " << app_option[E_]["description"] << std::endl;
            }
        }
    }
	if (!opt_.empty()) {
		std::cout << std::endl;
		sforeach(opt_) {
			String op_ = opHead(E_, app_option);
			std::cout << "  " << op_.filled(20) << ": " << app_option[E_]["description"] << std::endl;
		}
	}
	std::cout << std::endl;
	std::cout << SColorText("red", "  * Required option.") << std::endl;
	std::cout << SColorText("blue", "  # Choose one of the options.") << std::endl;
	std::cout << String(60, '=') << std::endl;
	return SAPP_EXECTED;
}
inline int showInfo(SDictionary& profile, const sobj& cmd) {
	std::cout << String("App Name:").filled(12) << profile["app"]["name"] << std::endl;
	std::cout << String("App Path:").filled(12) << sio::currentPath() << std::endl;
	std::cout << String("Creator:").filled(12) << profile["app"]["creator"] << std::endl;
	std::cout << String("Develop:").filled(12) << profile["app"]["develop"] << std::endl;
	std::cout << String("Version:").filled(12) << profile["app"]["version"] << std::endl;
	std::cout << String("License:").filled(12) << profile["app"]["license"] << std::endl;
	return SAPP_EXECTED;
}
inline int showVer(SDictionary& profile, const sobj& cmd) {
	std::cout << profile["app"]["name"] << " " << profile["app"]["version"] << std::endl;
	return SAPP_EXECTED;
}

SCuiApp::SCuiApp() : SApp() {}
SCuiApp::SCuiApp(const char *path) : SApp(path) {}
SCuiApp::SCuiApp(SDictionary &&prof) : SApp(std::forward<SDictionary &&>(prof)) {}
SCuiApp::~SCuiApp() {}

inline int getOption(String& op, const char* args) {
	if (args[0] == '-') {
		if (args[1] == '-') { op = &args[2]; return 2; }
		else { op = &args[1]; return 1; }
	}
	else if (args[0] == '>') return -1;
	else if (args[0] == '|') return -2;
	else { op = args; return 0; }
}
int SCuiApp::init(int argc, const char** argv) {
    try {
		int res;
		String op;
		const char** arg = &argv[1], ** argend = argv + argc;

		auto app_option = profile["option"];
		app_option["help"] = {
			kv("short", "H"),
			kv("description","Show help."),
			kv("type", "exec"),
			kv("func", SFunction<int(SDictionary&, const sobj&)>(showHelp))
		};
		app_option["info"] = {
			kv("short", "I"),
			kv("description","Show app information."),
			kv("type", "exec"),
			kv("func", SFunction<int(SDictionary&, const sobj&)>(showInfo))
		};
		app_option["version"] = {
			kv("short", "V"),
			kv("description","Show app version."),
			kv("type", "exec"),
			kv("func", SFunction<int(SDictionary&, const sobj&)>(showVer))
		};
		auto opts = app_option.keyset();
		sforeach(opts) {
			if (app_option[E_].hasKey("short")) app_option["_abbr"].set(app_option[E_]["short"],  E_);
		}
		if (profile["app"]["type"].intValue() == SCUI_SCMD_APP) {
			auto command = profile["command"]["_exec"];
			auto require = command["require"];
			sforeach(opts) {
				if (require && !require.empty()) {
					if(!require.contain(E_) && !command["option"].contain(E_)) command["option"].add(E_);
				}
				else if (!command["option"].contain(E_)) command["option"].add(E_);
			}
			preference["_cmd"] = "_exec";
		}
		else {
			if (argc < 2) throw SAppException(ERR_INFO, INSUFFICIENT_ARGS_ERROR, "application init", "<command>");
			res = getOption(op, arg[0]);
			switch (res) {
			case 0:
			{
				if (profile["command"].hasKey(op)) preference["_cmd"] = op;
				else throw SAppException(ERR_INFO, COMMAND_NOT_EXIST_ERROR, op);
				break;
			}
			case 1: {
				if (op.empty()) throw SAppException(ERR_INFO, OPTION_NOT_EXIST_ERROR, "option ''");
				if (app_option["_abbr"].hasKey(op)) {
					op = app_option["_abbr"][op];
					if (app_option[op]["type"] == "exec")
						return app_option[op]["func"].func<int(SDictionary&, const sobj&)>()(profile, snull);
				}
				throw SAppException(ERR_INFO, INSUFFICIENT_ARGS_ERROR, "application init", "<command>");
			}
			case 2: {
				if (app_option.hasKey(op) && app_option[op]["type"] == "exec")
					return app_option[op]["func"].func<int(SDictionary&, const sobj&)>()(profile, snull);
				throw SAppException(ERR_INFO, INSUFFICIENT_ARGS_ERROR, "application init", "<command>");
			}
			}
		}
		if (preference["_cmd"].isNull()) throw SAppException(ERR_INFO, INSUFFICIENT_ARGS_ERROR, "application init", "<command>");
		auto required = profile["command"][preference["_cmd"]]["require"];
		
		while (arg < argend) {
			res = getOption(op, arg[0]);
			switch (res) {
			case 0:
			{
				if (app_option.hasKey("_args")) {
					preference["_args"].add(op);
					if (required.contain("_args")) app_option["_args"]["_set"] = true;
				}
				break;
			}
			case 1: {
				if (op.empty()) throw SAppException(ERR_INFO, OPTION_NOT_EXIST_ERROR, "option ''");
				else if (1 < op.length()) {
					sforeach(op) {
						String op_(1, E_);
						if (app_option["_abbr"].hasKey(op_)) {
							String op__ = app_option["_abbr"][op_];
							if (required.contain(op__)) app_option[op__]["_set"] = true;
							if (app_option[op__]["type"] == "bool") preference[op__] = true;
							else throw SAppException(ERR_INFO, INSUFFICIENT_OPT_ERROR, op__, "arg");
						}
						else throw SAppException(ERR_INFO, OPTION_NOT_EXIST_ERROR, "option '" + op_ + "'");
					}
				}
				else {
					if (app_option["_abbr"].hasKey(op)) {
						op = app_option["_abbr"][op];
						if (required.contain(op)) app_option[op]["_set"] = true;
						if (app_option[op]["type"] == "exec")
							return app_option[op]["func"].func<int(SDictionary&, const sobj&)>()(profile, preference["_cmd"]);
						else if (app_option[op]["type"] == "bool") preference[op] = true;
						else if (app_option[op]["type"] == "num") { arg++; preference[op] = SNumber::toNumber(arg[0]); }
						else { arg++; preference[op] = arg[0]; }
					}
					else throw SAppException(ERR_INFO, OPTION_NOT_EXIST_ERROR, "option '" + op + "'");
				}
				break;
			}
			case 2: {
				if (app_option.hasKey(op)) {
					if (required.contain(op)) app_option[op]["_set"] = true;
					if (app_option[op]["type"] == "bool") preference[op] = true;
					else if (app_option[op]["type"] == "exec") 
						return app_option[op]["func"].func<int(SDictionary&, const sobj &)>()(profile, preference["_cmd"]);
					else {
						arg++; preference[op] = arg[0];
					}
				}
				else throw SAppException(ERR_INFO, OPTION_NOT_EXIST_ERROR, "option '" + op + "'");
				break;
			}
			case -1: {
				arg++;
				if (app_option.hasKey("_out")) {
					preference["_out"] = arg[0];
					if (required.contain("_out")) app_option["_out"]["_set"] = true;
				}
				else throw SAppException(ERR_INFO, OPTION_NOT_EXIST_ERROR, "export option '>'");
				break;
			}
			case -2: {
				arg++;
				if (app_option.hasKey("_pipe")) {
					while (arg < argend) { preference["_pipe"].add(arg[0]); ++arg; }
					if (required.contain("_pipe")) app_option["_pipe"]["_set"] = true;
				}
				else throw SAppException(ERR_INFO, OPTION_NOT_EXIST_ERROR, "pipe option '|'");
				break;
			}
			default:
				break;
			}
			++arg;
		}
		if (required && required.size()) {
            sforeach(required) {
				if (!app_option[E_]["_set"]) throw SAppException(ERR_INFO, INSUFFICIENT_OPT_ERROR, "application init", E_);
            }
        }
    } catch (SAppException ae) {
        ae.print();
		std::cout << std::endl;
        if (ae.err == COMMAND_NOT_EXIST_ERROR ||
            ae.err == OPTION_NOT_EXIST_ERROR) showHelp(profile, preference["_cmd"]);
        else if (ae.err == INSUFFICIENT_ARGS_ERROR ||
			ae.err == INSUFFICIENT_OPT_ERROR) showUsage(profile, preference["_cmd"]);
        return ae.err;
    }
    return 0;
}

int SCuiApp::run(int argc, const char **argv) {
	auto res = init(argc, argv);
	if (res) return res == SAPP_EXECTED ? 0 : res;
	else return exec();
}
