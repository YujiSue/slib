#ifndef SLIB_SMAIL_H
#define SLIB_SMAIL_H
#include "sbasic/ptr.h"
#include "sutil/scode.h"
#include "sutil/sjson.h"
#include "sutil/snet.h"
namespace slib {    
    class SLIB_DLL SMail;
    namespace smail {
        enum class FLAG : subyte {
            TO = 0x01,
            CC = 0x02,
            BCC = 0x03,

            NEW = 0x10,

            ALL = 0xFF,
        };
        constexpr subyte TEXT = 0x01;
        constexpr subyte ATTACHMENT = 0x10;

        class SLIB_DLL Message {
        public:
            String mailid, msgid, from, reply, subject;
            SDate date;
            Array<Pair<smail::FLAG, String>> to;
            SDictionary header;
            SArray body;

        public:
            Message();
            Message(const Message &msg);
            ~Message();
            Message& operator=(const Message& msg);
            void clear();
        };
        class SLIB_DLL MessageList;
        class SLIB_DLL MessageIterator {
            friend SMail;
            friend MessageList;
        private:
            MessageList* _container;
            ArrayIterator<String> _iter;
        public:
            MessageIterator(MessageList* ml, ArrayIterator<String> it);
            MessageIterator(const MessageIterator& mit);
            ~MessageIterator();
            MessageIterator &operator=(const MessageIterator& it);
            Message& operator*();
            MessageIterator& operator++();
            MessageIterator& operator--();
            MessageIterator& operator+=(std::ptrdiff_t diff);
            MessageIterator& operator-=(std::ptrdiff_t diff);
            MessageIterator operator+(std::ptrdiff_t diff);
            MessageIterator operator-(std::ptrdiff_t diff);
            std::ptrdiff_t operator-(const MessageIterator& it);
            bool operator<(const MessageIterator& it) const;
            bool operator==(const MessageIterator& it) const;
            bool operator!=(const MessageIterator& it) const;
        };

        class SLIB_DLL MessageList {
            friend SMail;
            friend MessageIterator;
        private:
            SMail* _mailer;
            smail::Message _msg;
        public:
            stringarray msgs;
            Map<String, stringarray> threads;
            SDictionary attribute;

        public:
            MessageList(SMail *m);
            ~MessageList();
            bool nextPage();
            smail::Message& getMessage(const String& mid);
            size_t size();
            MessageIterator begin();
            MessageIterator end();
        };

    }

    namespace sgmail {
        extern SLIB_DLL Response getAccessToken(slib::SNetWork &net, SDictionary &account, const SDictionary& prop);
        extern SLIB_DLL Response updateAccessToken(slib::SNetWork& net, SDictionary& account);
        extern SLIB_DLL Response getLabelList(slib::SNetWork& net, const SDictionary& account, SArray& mailboxes);
        extern SLIB_DLL Response getMailList(slib::SNetWork& net, const SDictionary& account, smail::MessageList& list);
        extern SLIB_DLL Response getMailMessage(slib::SNetWork& net, const SDictionary& account, smail::Message& msg, const String& mid);
        extern SLIB_DLL Response getAttachment(slib::SNetWork& net, const SDictionary& account, SData& content, const String& mid, const String& aid);
        extern SLIB_DLL void toMessages(smail::Message& msg, const SDictionary& dict);
    }

    
    class SLIB_DLL SMail {
        friend smail::MessageIterator;
        friend smail::MessageList;
    private:
        std::function<Response(slib::SNetWork&, SDictionary& , const SDictionary&)> _authenticate;
        std::function<Response(slib::SNetWork&, SDictionary&)> _authcheck;
        std::function<Response(slib::SNetWork&, const SDictionary&, SArray &)> _boxlist;
        std::function<Response(slib::SNetWork&, const SDictionary&, smail::MessageList&)> _loadlist;
        std::function<Response(slib::SNetWork&, const SDictionary&, smail::Message&, const String&)> _msgloader;
        std::function<Response(slib::SNetWork&, const SDictionary&, SData&, const String&, const String&)> _attachloader;

    protected:
        SNetWork _net;
        SDictionary _account;

    public:
        smail::MessageList messages;
        SArray mailboxes;

    public:
        SMail(const String &serv, sobj auth = snull);
        ~SMail();

        String accountInfo(const char* key) const;
        
        void authenticate(const SDictionary& auth);
        void loadList(const SDictionary& prop);
        void loadMessage(smail::Message& msg, const String& mid);
        void loadAttachment(SData& dat, const String& mid, const String& aid);
        //void send(smail::Message& msg);
    };
}

#endif