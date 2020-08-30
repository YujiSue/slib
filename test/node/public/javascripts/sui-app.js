var SAPP_INSTANCE=null;
var SAPP_USER=null;
var SAPP_PREF=new SCookie();
var SAPP_THEMA=SAPP_PREF.cookieAt('thema')?SAPP_PREF.cookieAt('thema'):'/stylesheets/sui-default.css';
document.getElementById('main-thema').href=SAPP_THEMA;
var SAPP_LANG=SAPP_PREF.cookieAt('lang')?SAPP_PREF.cookieAt('lang'):'en';
var SAPP_ROOT=null;
var SAPP_CONNECT=new SConnect();
var SAPP_SOCKET=null;
var SAPP_ABOUT=null;
var LANG_MENU=smenu({id:'lang-menu', style:S_SELECT});
for(var l in SupportedLocale) {LANG_MENU.addItem({style:S_TOGGLE|S_LABEL,state:SAPP_LANG===l,label:l,action:changeLocale})}
function changeThema(t) {
    SAPP_THEMA = t;
    document.getElementById('main-thema').href=SAPP_THEMA;
};
function changeLocale() {
    SAPP_LANG=SupportedLocale[LANG_MENU.selectedIndex()];
    if(SAPP_ROOT) SAPP_ROOT.setLocale(SAPP_LANG); 
    SAPP_PREF.setCookie('lang',SAPP_LANG);
};
function checkUser() {
    
}
function uiprop(i,p) {
    var prop={};
    if (i.I) prop.id=i;
    if (p.N) prop.name=p.N;
    if (p.C) prop.class=p.C;
    if (p.E) prop.element=p.E;
    if (p.L) {
        if (p.L==='HF') prop.layout=sflow(HORIZONTAL);
        else if (p.L==='VF') prop.layout=sflow(VERTICAL);
        else if (p.L==='HC') prop.layout=scard(HORIZONTAL);
        else if (p.L==='VC') prop.layout=scard(VERTICAL);
        else if (p.L==='B') prop.layout=sborder();
        else if (p.L==='G'&&p.G) prop.layout=sgrid(p.G);
    }
    if (p.D) prop.display=p.D;
    if (p.P) prop.position=p.P;
    if (p.B) prop.background=p.B;
    if (p.S) {
        prop.minw=p.S[0][0];prop.maxw=p.S[0][1];
        prop.minh=p.S[1][0];prop.maxh=p.S[1][1];
    }
    if (p.A) {
        for(k in p.A) {
            if(K==='sel') prop.selectable=p.A[k];
            if(K==='ed') prop.editable=p.A[k];
            if(K==='re') prop.resizable=p.A[k];
            if(K==='drg') prop.draggable=p.A[k];
            if(K==='mov') prop.movable=p.A[k];
        }
    }
    if (p.a) prop.action=p.a;
    if (p.e) prop.event=p.e;
    if (p.l) prop.locale=p.l;
    if (p.k) prop.shortcut=p.k;
    if (p.f) prop.font=p.f;
    if (p.s) prop.style=p.s;
    if (p.m) prop.mode=p.m;
    if (p.d) prop.direction=p.d;
    if (p.o) prop.expand=p.o;
    if (p.c) prop.cursor=p.c;
    if (p.v) prop.visible=p.v;
    if (p.u) prop.available=p.u;
    if (p.p) prop.placeholder=p.p;
    if (p.r) prop.required=p.r;
    if (p.x) prop.x=p.x;
    if (p.y) prop.y=p.y;
    if (p.z) prop.z=p.z;
    if (p.w) prop.width=p.w;
    if (p.h) prop.height=p.h;
    if (p.i) prop.icon=p.i;
    return prop;
};
function arrangeUI(uis) {
    for(var a in APP_UI.arrange) {
        const children = APP_UI.arrange[a];
        if(children instanceof Array) {
            for(var c=0;c<children.length;c++) {
                if(typeof children[c]==='string') uis[a].add(uis[children[c]]);
                else uis[a].add(children[c]);
            } 
        }
        else {
            for(var l in children) {
                for(var c=0;c<children[l].length;c++) {
                    if(typeof children[l][c]==='string') uis[a].add(uis[children[l][c]],l);
                    else uis[a].add(children[l][c],l);
                }
            }
        }
    }
};
function makeUI() {
    var uis={};
    for(var u in APP_UI.elements) {
        const info=APP_UI.elements[u];
        const ui=info.UI;
        info.prop=uiprop(u,info);
        if (ui==='E') uis[u]=suic(info.T,info.prop);
        else if (ui==='V') uis[u]=sview(info.prop);
        else if (ui==='P') uis[u]=spanel(info.prop);
        //else if (ui==='Fr') uis[u]=sframe(info.prop));
        //else if (ui==='W') uis[u]=swindow(info.prop));
        else if (ui==='Bx') uis[u]=sbox(info.T,info.prop);
        else if (ui==='ExV') uis[u]=sexview(info.prop);
        else if (ui==='TbV') uis[u]=new STabView(info.prop);
        else if (ui==='Li') uis[u]=slist(info.prop);
        else if (ui==='LV') uis[u]=sliview(info.prop);
        else if (ui==='CV') uis[u]=scardview(info.prop);
    
        else if (ui==='Im') uis[u]=simgview(info.prop);
        else if (ui==='Wb') uis[u]=swebview(info.prop);
    
        else if (ui==='Mi') {
            if(info.M) info.prop.submenu = uis[info.M];
            uis[u]=smenuitem(info.prop);
        }
        else if (ui==='M') uis[u]=smenu(info.prop);
        else if (ui==='Mb') uis[u]=smenubar(info.prop);
        else if (ui==='TI') uis[u]=stoolitem(info.prop);
        else if (ui==='TB') uis[u]=stoolbar(info.prop);

        else if (ui==='F') uis[u]=sform({method:info.O,action:info.V});
        else if (ui==='I') uis[u]=sicon(info.O,info.prop);
        else if (ui==='L') uis[u]=slabel(info.T,info.prop);
        else if (ui==='LL') uis[u]=slink(info.T,info.V,info.prop);
        else if (ui==='In') uis[u]=sinput(info.O,info.prop);
        else if (ui==='TF') uis[u]=stextfield(info.prop);
    
        else if (ui==='B') uis[u]=sbutton(info.prop);
        else if (ui==='CB') uis[u]=scheck(info.prop);
        else if (ui==='RB') uis[u]=sradio(info.prop);
        else if (ui==='SEL') uis[u]=sselect(info.prop);
        else if (ui==='SL') uis[u]=sslider(info.prop);
        else if (ui==='FL') uis[u]=new SFileLoader(info.prop);
        else if (ui==='DP') uis[u]=new SDatePicker(info.prop);
        else if (ui==='CP') uis[u]=new SColorPicker(info.prop);
        
        else if (ui==='Bar') uis[u]=sbar(info.d);
        else if (ui==='SP') uis[u]=sspace();
    }
    arrangeUI(uis);
    return uis;
};

function makeAboutPane() {

    return sdialog({
        style:GENERIC_DIALOG,
        layout:sflow(VERTICAL),
        class:['app-about'],
        components:[
            sview({
                class:['upper-panel'],
                layout:sflow(HORIZONTAL),
                components:[
                    simgview({src:APP_INFO.icon,class: ['app-icon']}),
                    spanel({
                        layout:sflow(VERTICAL),
                        class:['auto'],
                        components:[
                            slabel(APP_INFO.name,{class:['app-name']}),
                            slabel('ver. '+APP_INFO.version),
                            slink('Website',APP_INFO.site,{})
                        ]
                    })

                ]
            }),
            sview({
                class:['lower-panel'],
                components:[slabel(APP_INFO.license+' '+COPYWRITE_CHAR+' '+APP_INFO.develop.substr(0,APP_INFO.develop.indexOf('/'))+' '+APP_INFO.creator,{class:['app-copy']})]
            })
        ]
    });
}
function SPrefPane(pref) {
    var pane=new SDialog({
        modal: true,
        class: ['app-pref']
    });
    var form=new SForm(pref.form);
    var items=Array(pref.ui.length);
    for (var i=0; i < pref.ui.length; i++) {
        items[i]=makeUI(pref.ui[i]);
    }
    rearrange(items, pref.arrange);
    for (var i=0; i < pref.arrange.length; i++) {
        form.add(items[pref.arrange[i].parent]);
    }
    pane.frame.add(form);
    var bPane=spane({
        layout: new SFlowLayout(HORIZONTAL)
    });
    var impBtn=sbutton({
        label: 'import',
        class: ['pref-import'],
        available: false
    });
    var expBtn=sbutton({
        label: 'export',
        class: ['pref-export'],
        available: false
    });
    var cancelBtn=sbutton({
        label: 'cancel',
        class: ['pref-cancel'],
        action: function() { pane.hide(); }
    });
    var saveBtn=sbutton({
        label: 'save',
        class: ['pref-save'],
        action: function() {form.submit(); pane.hide();}
    });
    bPane.add(impBtn).add(expBtn).add(sspace()).add(cancelBtn).add(saveBtn);
    pane.frame.add(buttonPane);
};
SPrefPane.prototype={
    show: function(b) {
        if (b) pane.show();
        else pane.hide();
    }
};
function SApp() {
    SAPP_INSTANCE=this;
    this.uis = makeUI();
    SAPP_ROOT = this.uis.root;
    document.body.appendChild(SAPP_ROOT.node);
    SAPP_ABOUT = makeAboutPane(APP_INFO);
    SAPP_ROOT.add(SAPP_ABOUT.screen,'center');


    SAPP_SOCKET = io();
    SAPP_SOCKET.on('notify',function(msg) {
        console.log(msg);
    });
    SAPP_SOCKET.emit('sender','socket test');

    this.prefPane=null;
};
SApp.prototype={
    showAbout: function() {
        if(SAPP_ABOUT) SAPP_ABOUT.show();
    },
    showPref: function() {
        this.prefPane.show(true);
    },
    setLang: function(l) {
        SAPP_LANG = l;
        if (SAPP_ROOT) SAPP_ROOT.setLocale(SAPP_LANG);
    },
    setThema: function(t) {
        document.getElementById('main-thema').href=t;
    }
};