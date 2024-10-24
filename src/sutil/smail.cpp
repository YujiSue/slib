#include "sutil/smail.h"
slib::smail::Message::Message() {}
slib::smail::Message::Message(const slib::smail::Message& msg) {
	msgid = msg.msgid;
	mailid = msg.mailid;
	from = msg.from;
	reply = msg.reply;
	subject = msg.subject;
	to = msg.to;
	date = msg.date; 
	header = msg.header;
	body = msg.body;
}
slib::smail::Message::~Message() {}
slib::smail::Message& slib::smail::Message::operator=(const slib::smail::Message& msg) {
	mailid = msg.mailid;
	msgid = msg.msgid;
	from = msg.from;
	reply = msg.reply;
	subject = msg.subject;
	to = msg.to;
	date = msg.date;
	header = msg.header;
	body = msg.body;
	return *this;
}
void slib::smail::Message::clear() {
	mailid.clear();
	msgid.clear();
	from.clear();
	reply.clear();
	subject.clear();
	to.clear();
	//date
	header.clear();
	body.clear();
}
slib::smail::MessageIterator::MessageIterator(MessageList* ml, ArrayIterator<String> it) : _container(ml), _iter(it) {}
slib::smail::MessageIterator::MessageIterator(const MessageIterator& it) {
	_container = it._container; _iter = it._iter;
}
slib::smail::MessageIterator::~MessageIterator() {}
slib::smail::MessageIterator& slib::smail::MessageIterator::operator=(const MessageIterator& it) {
	_container = it._container; _iter = it._iter; return *this;
}
slib::smail::Message& slib::smail::MessageIterator::operator*() {
	if (_container->_msg.mailid == *_iter) return _container->_msg;
	return _container->getMessage(*_iter);
}
slib::smail::MessageIterator& slib::smail::MessageIterator::operator++() {
	if (_iter == _container->msgs.end() - 1 &&
		_container->nextPage()) {
		_iter = _container->msgs.begin();
	}
	else ++_iter;
	return *this;
}
slib::smail::MessageIterator& slib::smail::MessageIterator::operator--() {
	if (_container->msgs.begin() < _iter) --_iter;
	return *this;
}
slib::smail::MessageIterator& slib::smail::MessageIterator::operator+=(std::ptrdiff_t diff) {
	sforin(i, 0, diff) ++(*this);
	return *this;
}
slib::smail::MessageIterator& slib::smail::MessageIterator::operator-=(std::ptrdiff_t diff) {
	sforin(i, 0, diff) --(*this);
	return *this;
}
slib::smail::MessageIterator slib::smail::MessageIterator::operator+(std::ptrdiff_t diff) {
	MessageIterator mit(*this); mit += diff; return mit;
}
slib::smail::MessageIterator slib::smail::MessageIterator::operator-(std::ptrdiff_t diff) {
	MessageIterator mit(*this); mit -= diff; return mit;
}
std::ptrdiff_t slib::smail::MessageIterator::operator-(const MessageIterator& it) {
	return _iter - it._iter;
}
bool slib::smail::MessageIterator::operator<(const MessageIterator& it) const { return _iter < it._iter; }
bool slib::smail::MessageIterator::operator==(const MessageIterator& it) const { return _iter == it._iter; }
bool slib::smail::MessageIterator::operator!=(const MessageIterator& it) const { return !(_iter == it._iter); }

slib::smail::MessageList::MessageList(SMail* m) : _mailer(m) {}
slib::smail::MessageList::~MessageList() {}
bool slib::smail::MessageList::nextPage() {
	if (attribute["page"]) {
		_mailer->loadList({});
		return true;
	}
	return false;
}
slib::smail::Message& slib::smail::MessageList::getMessage(const String& mid) {
	_mailer->loadMessage(_msg, mid);
	return _msg;
}
size_t slib::smail::MessageList::size() { return msgs.size(); }
slib::smail::MessageIterator slib::smail::MessageList::begin() { 
	return MessageIterator(this, msgs.begin());
}
slib::smail::MessageIterator slib::smail::MessageList::end() {
	return MessageIterator(this, msgs.end());
}

slib::SMail::SMail(const String& serv, sobj auth) : messages(this) {
	if (serv == "gmail") {
		_authenticate = sgmail::getAccessToken;
		_authcheck = sgmail::updateAccessToken;
		_boxlist = sgmail::getLabelList;
		_loadlist = sgmail::getMailList;
		_msgloader = sgmail::getMailMessage;
		_attachloader = sgmail::getAttachment;
	}
	if (auth) authenticate(auth);
}
slib::SMail::~SMail() {}
slib::String slib::SMail::accountInfo(const char* key) const {
	return _account[key].toString();
}
void slib::SMail::authenticate(const SDictionary& auth) { 
	_authenticate(_net, _account, auth);
	_boxlist(_net, _account, mailboxes);
}
void slib::SMail::loadList(const SDictionary& prop) {
	_authcheck(_net, _account);
	messages.attribute.update(prop);
	_loadlist(_net, _account, messages);
}
void slib::SMail::loadMessage(smail::Message& msg, const String& mid) {
	_authcheck(_net, _account);
	_msgloader(_net, _account, msg, mid);
	sfor(msg.body) {
		if ($_["type"] == (int)smail::ATTACHMENT) 
			_attachloader(_net, _account, $_["data"].data(), mid, $_["id"]);
	}
}
void slib::SMail::loadAttachment(SData& dat, const String& mid, const String& aid) {
	_authcheck(_net, _account);
	_attachloader(_net, _account, dat, mid, aid);
}
slib::Response slib::sgmail::getAccessToken(slib::SNetWork& net, SDictionary& account, const SDictionary& prop) {
	net.clear();
	if (prop.hasKey("credentials")) {
		String buf;
		SFile(prop["credentials"]) >> buf;
		account.append(sjson::parse(buf)["installed"]);
	}
	if (prop.hasKey("code")) account["code"] = prop["code"];
	if (prop.hasKey("refresh_token")) account["refresh_token"] = prop["refresh_token"];
	if (account.hasKey("refresh_token")) sgmail::updateAccessToken(net, account);
	else {
		auto res = net.connect("https://www.googleapis.com/oauth2/v4/token", {
		D_("payload", sobj({
			D_("client_id", account["client_id"]),
			D_("client_secret", account["client_secret"]),
			D_("redirect_uri", account["redirect_uris"][0]),
			D_("grant_type", "authorization_code"),
			D_("code", account["code"])
			}))
			}).json();


		account["access_token"] = res["access_token"];
		account["refresh_token"] = res["refresh_token"];
	}
	return Response();
}
slib::Response slib::sgmail::updateAccessToken(slib::SNetWork& net, SDictionary& account) {
	net.clear();
	auto res = net.connect("https://www.googleapis.com/oauth2/v4/token", {
		D_("payload", sobj({
			D_("client_id", account["client_id"]),
			D_("client_secret", account["client_secret"]),
			D_("refresh_token", account["refresh_token"]),
			D_("grant_type", "refresh_token")
			}))
		}).json();

	if (res.hasKey("error")) {
		SPrint(res);
		getAccessToken(net, account, {});
	}

	account["access_token"] = res["access_token"];
	return Response();
}
slib::Response slib::sgmail::getLabelList(slib::SNetWork& net, const SDictionary& account, SArray& mailboxes) {
	net.clear();
	auto res = net.connect("https://gmail.googleapis.com/gmail/v1/users/me/labels", {
		D_("header", sobj({
			D_("Authorization", S("Bearer ") + account["access_token"])
			}))
		}).json();
	
	if (res.hasKey("error")) {
		SPrint(res);
		throw Exception();
	}

	mailboxes = res["labels"];
	return Response();
}
slib::Response slib::sgmail::getMailList(slib::SNetWork& net, const SDictionary& account, smail::MessageList& list) {
	net.clear();
	auto url = S("https://gmail.googleapis.com/gmail/v1/users/me/messages?");
	if (list.attribute.hasKey("label")) url << "labelIds=" << list.attribute["label"] << "&";
	if (list.attribute.hasKey("page")) url << "pageToken=" << list.attribute["page"] << "&";
	//
	url.resize(url.size() - 1);
	auto res = net.connect(url, {
		D_("header", sobj({
			D_("Authorization", S("Bearer ") + account["access_token"])
			}))
		}).json();

	if (res.hasKey("error")) {
		SPrint(res);
		throw Exception();	
	}

	if (res.hasKey("nextPageToken")) list.attribute["page"] = res["nextPageToken"];
	else list.attribute["page"] = snull;
	list.msgs.resize(res["messages"].size());
	sfor2(list.msgs, res["messages"]) { 
		$_1 = $_2["id"]; 
		if (list.threads.hasKey($_2["threadId"])) list.threads[$_2["threadId"]].add($_1);
		else list.threads[$_2["threadId"]] = { $_1 };
	}
	return Response();
}
slib::Response slib::sgmail::getMailMessage(slib::SNetWork& net, const SDictionary& account, smail::Message& msg, const String& mid) {
	net.clear();
	msg.clear();
	auto res = net.connect("https://gmail.googleapis.com/gmail/v1/users/me/messages/" + mid, {
		D_("header", sobj({
			D_("Authorization", S("Bearer ") + account["access_token"])
			}))
		}).json();

	if (res.hasKey("error")) {
		SPrint("> ", mid);
		SPrint(res);
		throw Exception();
	}

	msg.mailid = mid;
	toMessages(msg, res);
	return Response();
}
slib::Response slib::sgmail::getAttachment(slib::SNetWork& net, const SDictionary& account, SData& content, const String& mid, const String& aid) {
	net.clear();
	content.clear();
	auto res = net.connect("https://gmail.googleapis.com/gmail/v1/users/me/messages/" + mid + "/attachments/" + aid, {
		D_("header", sobj({
			D_("Authorization", S("Bearer ") + account["access_token"])
			}))
		}).json();

	if (res.hasKey("error")) {
		SPrint("> ", mid, NL, " >> ", aid);
		SPrint(res);
		throw Exception();
	}

	slib::sutil::decodeBase64(res["data"], content);
	content.resize(res["size"]);
	return Response();
}
inline slib::SObjPtr _parseMail(const slib::SDictionary &content) {
	slib::SObjPtr obj = { D_("mime", content["mimeType"]) };
	if (content["body"].hasKey("attachmentId")) {
		obj["type"] = slib::smail::ATTACHMENT;
		obj["id"] = content["body"]["attachmentId"];
		obj["name"] = content["filename"];
		obj["data"] = slib::SData();
	}
	else {
		obj["type"] = slib::smail::TEXT;
		slib::String tmp;
		slib::sutil::decodeBase64(content["body"]["data"], tmp);
		obj["text"] = tmp;
	}
	return obj;
}
void slib::sgmail::toMessages(smail::Message& msg, const SDictionary& mail) {
	sforeach(header, mail["payload"]["headers"]) {
		if (header.hasKey("name") && header.hasKey("value")) {
			//SPrint("H:", header["name"]);
			if (header["name"] == "Message-ID") msg.msgid = header["value"];
			else if (header["name"] == "From") msg.from = header["value"];
			else if (header["name"] == "To") {
				auto list = header["value"].split(",");
				sforeach(li, list) {
					msg.to.add(smail::FLAG::TO, li.trim());
				}
			}
			else if (header["name"] == "Subject") msg.subject = header["value"];
			else if (header["name"] == "Date") msg.date = SDate(header["value"], sstyle::EMAIL);
			else msg.header[header["name"]] = header["value"];

		}
	}
	if (mail["payload"]["body"]["size"] == 0 && mail["payload"]["parts"].size()) {
		msg.body.resize(mail["payload"]["parts"].size());
		sfor2(msg.body, mail["payload"]["parts"]) { $_1 = _parseMail($_2); }
	}
	else {
		msg.body.resize(1);
		msg.body[0] = _parseMail(mail["payload"]);
	}
}
