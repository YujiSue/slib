#include "sapp/scuiapp.h"
inline void outputOp(sobj& option, sobj& list, bool op) {
	slib::String sop, lop;
	sfor(list) {
		if ($_.beginWith("_") || !option.hasKey($_)) continue;
		if (option[$_]["type"] == "exec") {
			continue;
		}
		else if (option[$_]["type"] == "bool") {
			if (option[$_].hasKey("short")) sop += option[$_]["short"];
			else lop << (op ? " { --" : " --") << $_ << (op ? " }" : "");
		}
		else {
			if (option[$_].hasKey("short")) lop << (op ? " { -" : " -") << option[$_]["short"] << " ";
			else lop << (op ? " { --" : " --") << $_ << " ";
			lop << slib::stext::ITALIC_TAG << ((option[$_].hasKey("brief") ? option[$_]["brief"] : "arg")) << slib::stext::PLAIN_TAG << (op ? " }" : "");
		}
	}
	if (!sop.empty()) {
		SWrite((op ? " { " : " "), "-", sop, (op ? " }" : ""));
		if (!lop.empty()) SWrite(lop);
	}
	else if (!lop.empty())  SWrite(lop);
}
inline void outputSelectOp(sobj& option, sobj& list) {
	SWrite(" << ");
	sfor(list) {
		if ($_.beginWith("_") || !option.hasKey($_)) continue;
		if (option[$_]["type"] == "exec") continue;
		if (option[$_].hasKey("short")) SWrite("[ -", option[$_]["short"]);
		else SWrite("[ --", $_);
		if (option[$_]["type"] == "bool") SWrite(" ]");
		else SWrite(slib::stext::ITALIC_TAG, (option[$_].hasKey("brief") ? option[$_]["brief"] : "arg"), slib::stext::PLAIN_TAG, " ]");
		if (it < list.end() - 1) SWrite(" or ");
	}
	SWrite(" >> ");
}
inline void showUsage(const slib::SDictionary& profile, sobj cmd) {
	SPrint("Usage:");
	SWrite(slib::SP * 2, profile["info"]["name"]);
	sobj app_command;
	if (profile["info"]["type"] == "cui1") app_command = profile["command"]["_exec_"];
	else {
		if (!cmd) { SPrint(" <command> { options }"); return; }
		else {
			SWrite(slib::SP, cmd);
			app_command = profile["command"][cmd];
		}
	}
	auto app_option = profile["option"];
	if (app_command["require"]) outputOp(app_option, app_command["require"], false);
	if (app_command["select"]) outputSelectOp(app_option, app_command["select"]);
	if (app_command["option"]) outputOp(app_option, app_command["option"], true);
	if (app_command["require"].include("_args_"))
		SWrite(slib::SP, slib::stext::ITALIC_TAG, (app_option["_args_"].hasKey("brief") ? app_option["_args_"]["brief"] : "argments..."), slib::stext::PLAIN_TAG);
	if (app_command["require"].include("_out_"))
		SWrite(" > ", slib::stext::ITALIC_TAG, (app_option["_out_"].hasKey("brief") ? app_option["_out_"]["brief"] : "output"), slib::stext::PLAIN_TAG);
	if (app_command["require"].include("_pipe_"))
		SWrite(" | ", slib::stext::ITALIC_TAG, (app_option["_pipe_"].hasKey("brief") ? app_option["_pipe_"]["brief"] : "commands..."), slib::stext::PLAIN_TAG);
	if (app_command["option"].include("_args_"))
		SWrite(" { ", slib::stext::ITALIC_TAG, (app_option["_args_"].hasKey("brief") ? app_option["_args"]["brief"] : "argments..."), slib::stext::PLAIN_TAG, " }");
	if (app_command["option"].include("_out_"))
		SWrite(" { > ", slib::stext::ITALIC_TAG, (app_option["_out_"].hasKey("brief") ? app_option["_out"]["brief"] : "output"), slib::stext::PLAIN_TAG, " }");
	if (app_command["option"].include("_pipe_"))
		SWrite(" { | ", slib::stext::ITALIC_TAG, (app_option["_pipe_"].hasKey("brief") ? app_option["_pipe"]["brief"] : "commands..."), slib::stext::PLAIN_TAG, " }");
	SPrint("");
}
inline slib::String opHead(const sobj &key, const sobj& option) {
	return S_(--) << key << (option[key].hasKey("short") ? S_(/-) + option[key]["short"] : "");
}
inline void showHelp(const slib::SDictionary& profile, sobj cmd) {
	SPrint(slib::String(60, '='));
	SPrint(profile["info"]["name"], " v", profile["info"]["version"]);
	SPrint("Copyright (c) ", slib::SDate(profile["info"]["develop"]).year, slib::SP,
		profile["info"]["copyright"], ". All rights reserved. ", slib::NL,
		profile["info"]["license"]);
	SPrint(S_(. ) * 30, slib::NL);
	showUsage(profile, cmd);
	SPrint(S_(.) * 30, slib::NL * 2);
	auto app_command = profile["command"];
	auto app_option = profile["option"];
	stringarray options;
	sfor(app_option) {
		if ($_.value()["type"] == "exec") options.add($_.key());
	}
	if (profile["info"]["type"] == "cui2") {
		if (!cmd) {
			SPrint("[Command]");
			auto cmds = app_command.keyset();
			cmds.sort();
			sfor(cmds) {
				SPrint(slib::SP * 2, slib::sstr::fill($_, ' ', 16, slib::DIRECTION::TAIL), ": ", app_command[$_]["description"]);
				auto prof_ = profile["command"][$_];
			}
		}
		else {
			SPrint("[Option]");
			auto command = app_command[cmd];
			auto requires = command["require"];
			auto selects = command["select"];
			auto opts = command["option"];
			if (requires && !requires.empty()) {
				sfor(requires) {
					if ($_.beginWith("_") || app_option[$_]["type"] == "exec") continue;
					slib::String opt = opHead($_, app_option);
					SPrint(slib::SP * 2, slib::sstr::fill(opt, ' ', 20, slib::DIRECTION::TAIL), ": ",
						slib::stext::RED_TAG, "*", slib::stext::DEFAULT_COLOR,
						app_option[$_]["description"]);
				}
			}
			if (opts && !opts.empty()) {
				sfor(opts) {
					if ($_.beginWith("_") || app_option[$_]["type"] == "exec") continue;
					slib::String opt = opHead($_, app_option);
					SPrint(slib::SP * 2, slib::sstr::fill(opt, ' ', 20, slib::DIRECTION::TAIL), ":", slib::SP * 2, app_option[$_]["description"]);
					if (app_option[$_].hasKey("default")) SPrint(slib::SP * 25, "default=", slib::SP, app_option[$_]["default"]);
				}
			}
			if (selects && !selects.empty()) {
				sfor(selects) {
					if ($_.beginWith("_") || app_option[$_]["type"] == "exec") continue;
					slib::String op_ = opHead($_, app_option);
					SPrint(slib::SP * 2, slib::sstr::fill(op_, ' ', 20, slib::DIRECTION::TAIL), ": ", slib::stext::BLUE_TAG, "#", app_option[$_]["description"], slib::stext::DEFAULT_COLOR);
					if (app_option[$_].hasKey("default")) SPrint(slib::SP * 25, "default= ", app_option[$_]["default"]);
				}
			}
		}
	}
	else {
		if (!app_option.empty()) {
			SPrint("[Option]");
			auto command = profile["command"]["_exec"];
			auto required = command["require"];
			auto select = command["select"];
			auto opts = app_option.keyset();
			opts.sort();
			sfor(opts) {
				if ($_.beginWith("_") || app_option[$_]["type"] == "exec") continue;
				slib::String op_ = opHead($_, app_option);
				if (required.include($_)) SPrint(slib::SP * 2, slib::sstr::fill(op_, ' ', 20, slib::DIRECTION::TAIL), ": ", slib::stext::RED_TAG, "*", slib::stext::DEFAULT_COLOR, app_option[$_]["description"]);
				else if (select.include($_)) SPrint(slib::SP * 2, slib::sstr::fill(op_, ' ', 20, slib::DIRECTION::TAIL), ": ", slib::stext::BLUE_TAG, "#", slib::stext::DEFAULT_COLOR, app_option[$_]["description"]);
				else SPrint(slib::SP * 2, slib::sstr::fill(op_, ' ', 20, slib::DIRECTION::TAIL), ":  ", app_option[$_]["description"]);
				if (app_option[$_].hasKey("default")) SPrint(slib::SP * 25, "default= ", app_option[$_]["default"]);
			}
		}
	}
	if (!options.empty()) {
		SWrite(slib::NL);
		sfor(options) {
			slib::String op_ = opHead($_, app_option);
			SPrint(slib::SP * 2, slib::sstr::fill(op_, ' ', 20, slib::DIRECTION::TAIL), ": ", app_option[$_]["description"]);
		}
	}
	SWrite(slib::NL);
	SPrint(slib::SP * 2, slib::stext::RED_TAG, "* Required option.", slib::stext::DEFAULT_COLOR);
	SPrint(slib::SP * 2, slib::stext::BLUE_TAG, "# Choose one of the options.", slib::stext::DEFAULT_COLOR);
	SPrint(slib::String(60, '='));
}
inline void showInfo(const slib::SDictionary& profile, sobj cmd) {
	SPrint(slib::sstr::fill("App Name:", ' ', 12, slib::DIRECTION::TAIL), profile["info"]["name"]);
	SPrint(slib::sstr::fill("App Path:", ' ', 12, slib::DIRECTION::TAIL), slib::ssys::current());
	SPrint(slib::sstr::fill("Creator:", ' ', 12, slib::DIRECTION::TAIL), profile["info"]["creator"]);
	SPrint(slib::sstr::fill("Develop:", ' ', 12, slib::DIRECTION::TAIL), profile["info"]["develop"]);
	SPrint(slib::sstr::fill("Version:", ' ', 12, slib::DIRECTION::TAIL), profile["info"]["version"]);
	SPrint(slib::sstr::fill("License:", ' ', 12, slib::DIRECTION::TAIL), profile["info"]["license"]);
}
inline void showVer(const slib::SDictionary& profile, sobj cmd) {
	SPrint(profile["info"]["name"], slib::SP, profile["info"]["version"]);
}
slib::sapp::SCuiApp::SCuiApp() : SApp() {}
slib::sapp::SCuiApp::SCuiApp(const char* prof, const char* pform) : SApp(prof, pform) {}
slib::sapp::SCuiApp::SCuiApp(slib::SDictionary&& prof) : SApp(std::forward<SDictionary&&>(prof)) {}
slib::sapp::SCuiApp::~SCuiApp() {}
int getOption(slib::String& op, const char* args) {
	if (args[0] == '-') {
		if (args[1] == '-') { op = &args[2]; return 2; }
		else { op = &args[1]; return 1; }
	}
	else if (args[0] == '>') return -1;
	else if (args[0] == '|') return -2;
	else { op = args; return 0; }
}
int slib::sapp::SCuiApp::init(int argc, const char** argv) {
	int res;
	const char** arg = &argv[1], ** argend = argv + argc;
	String op;
	SDictionary app_option = profile["option"];
	// Set default options
	app_option["help"] = {
			D_("short", "H"),
			D_("description","Show help."),
			D_("type", "exec"),
			D_("func", sobj(SFunction<void, const SDictionary&, const SObjPtr&>(showHelp)))
	};
	app_option["info"] = {
			D_("short", "I"),
			D_("description","Show app information."),
			D_("type", "exec"),
			D_("func", sobj(SFunction<void, const SDictionary&, const SObjPtr&>(showInfo)))
	};
	app_option["version"] = {
			D_("short", "V"),
			D_("description","Show version."),
			D_("type", "exec"),
			D_("func", sobj(SFunction<void, const SDictionary&, const SObjPtr&>(showVer)))
	};
	app_option["silent"] = {
			D_("short", "S"),
			D_("description","Silent mode (No output to stdout)"),
			D_("default", false),
			D_("type", "bool")
	};
	app_option["verbose"] = {
			D_("short", "v"),
			D_("description","Show log text."),
			D_("default", false),
			D_("type", "bool")
	};
	// 
	auto opts = app_option.keyset();
	sfor(opts) {
		// Map short options
		if (app_option[$_].hasKey("short")) app_option["_abbr_"][app_option[$_]["short"]] = $_;
		// Set default value to preference
		if (app_option[$_].hasKey("default")) preference[$_] = app_option[$_]["default"];
	}
	// For single command CUI app
	if (profile["info"]["type"] == "cui1") {
		auto cmd_info = profile["command"]["_"];
		auto require = cmd_info["require"];
		sfor(opts) {
			if (require && !require.empty()) {
				if (!require.include($_) && !cmd_info["option"].include($_)) cmd_info["option"].add($_);
			}
			else if (!cmd_info["option"].include($_)) cmd_info["option"].add($_);

		}
		preference["_cmd_"] = "_";
	}
	// For multiple commands CUI app
	else {
		if (argc < 2) throw InsufficientArgsException("<command>");
		res = getOption(op, arg[0]);
		switch (res) {
		case 0:
		{
			if (profile["command"].hasKey(op)) preference["_cmd_"] = op;
			else throw slib::AppCommandException(op);
			break;
		}
		case 1: {
			if (op.empty()) throw UndefinedOptException("(empty)");
			if (app_option["_abbr_"].hasKey(op)) {
				op = app_option["_abbr_"][op];
				if (app_option[op]["type"] == "exec") {
					auto& fnc = app_option[op]["func"].function<void, const SDictionary&, const SObjPtr&>();
					fnc(profile, snull);
					return 1;
				}
			}
			else throw slib::AppCommandException(op);
		}
		case 2: {
			if (app_option.hasKey(op) && app_option[op]["type"] == "exec") {
				auto& fnc = app_option[op]["func"].function<void, const SDictionary&, const SObjPtr&>();
				fnc(profile, snull);
				return 1;
			}	
			else throw slib::AppCommandException(op);
		}
		}
		++arg;
	}
	if (!preference["_cmd_"]) throw InsufficientArgsException("<command>");
	auto required = profile["command"][preference["_cmd_"]]["require"];
	while (arg < argend) {
		res = getOption(op, arg[0]);
		switch (res) {
		case 0:
		{
			if (app_option.hasKey("_args_")) {
#ifdef WIN_OS
				preference["_args_"].add(slib::String::toUTF8(op));
#else
				preference["_args_"].add(op);
#endif
				if (required.include("_args_")) app_option["_args_"]["_set_"] = true;
			}
			break;
		}
		case 1: 
		{
			if (op.empty()) throw UndefinedOptException("(empty)");
			else {
				sforc(op) {
					auto sop = $_.toString();
					if (app_option["_abbr_"].hasKey(sop)) {
						sop = app_option["_abbr_"][sop];
						if (required.include(sop)) app_option[sop]["_set_"] = true;
						if (app_option[sop]["type"] == "exec") {
							auto& fnc = app_option[sop]["func"].function<void, const SDictionary&, const SObjPtr&>();
							fnc(profile, preference["_cmd_"]);
							return 1;
						}
						else if (app_option[sop]["type"] == "bool") preference[sop] = true;
						else if (app_option[sop]["type"] == "num") { arg++; preference[sop] = N(arg[0]); }
						else if (app_option[op]["type"] == "array") {
							arg++;
							if (!preference.hasKey(sop)) preference[sop] = SArray();
#ifdef WIN_OS
							preference[sop].add(slib::String::toUTF8(arg[0]));
#else
							preference[sop].add(arg[0]);
#endif

						}
						else {
							arg++; 
#ifdef WIN_OS
							preference[sop] = slib::String::toUTF8(arg[0]);
#else
							preference[sop] = arg[0];
#endif
						}
					}
					else throw UndefinedOptException(sop);
				}
			}
			break;
		}
		case 2: 
		{
			if (app_option.hasKey(op)) {
				if (required.include(op)) app_option[op]["_set_"] = true;
				if (app_option[op].hasKey("defualt")) preference[op] = app_option[op]["default"];
				if (app_option[op]["type"] == "bool") preference[op] = true;
				else if (app_option[op]["type"] == "num") { arg++; preference[op] = N(arg[0]); }
				else if (app_option[op]["type"] == "exec") {
					auto& fnc = app_option[op]["func"].function<void, const SDictionary&, const SObjPtr&>();
					fnc(profile, preference["_cmd_"]);
					return 1;
				}
				else if (app_option[op]["type"] == "array") { 
					arg++; 
					if (!preference.hasKey(op)) preference[op] = SArray();
#ifdef WIN_OS
					preference[op].add(slib::String::toUTF8(arg[0]));
#else
					preference[op].add(arg[0]);
#endif

				}
				else {
					arg++; 
#ifdef WIN_OS
					preference[op] = slib::String::toUTF8(arg[0]);
#else
					preference[op] = arg[0];
#endif
				}
			}
			else throw UndefinedOptException(op);
			break;
		}
		case -1: {
			arg++;
			if (app_option.hasKey("_out_")) {
#ifdef WIN_OS
				preference["_out_"] = slib::String::toUTF8(arg[0]);
#else
				preference["_out_"] = arg[0];
#endif
				if (required.include("_out_")) app_option["_out_"]["_set_"] = true;
			}
			else throw UndefinedOptException(">");
			break;
		}
		case -2: {
			arg++;
			if (app_option.hasKey("_pipe_")) {
				while (arg < argend) { preference["_pipe_"].add(arg[0]); ++arg; }
				if (required.include("_pipe_")) app_option["_pipe_"]["_set_"] = true;
			}
			else throw UndefinedOptException("|");
			break;
		}
		default:
			break;
		}
		++arg;
	}
	if (required && required.size()) {
		sfor(required) {
			if (!app_option[$_]["_set_"]) throw InsufficientArgsException($_.toString());
		}
	}
	return 0;
}
int slib::sapp::SCuiApp::run(int argc, const char** argv) {
	try {
		auto res = init(argc, argv);
		if (res) return res == sapp::COMPLETE ? 0 : res;
		else return exec();
	}
	catch (SAppException ae) { ae.print(); return ae.code; }
}