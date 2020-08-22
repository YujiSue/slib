var APP_INSTANCE=null;
var APP_PREF=new SCookie();
var APP_THEMA=APP_PREF.cookieAt('thema')?APP_PREF.cookieAt('thema'):'/stylesheets/sui-default.css';
document.getElementById('main-thema').href=APP_THEMA;
var APP_LANG=APP_PREF.cookieAt('lang')?APP_PREF.cookieAt('lang'):'en';
var APP_ROOT=null;
var APP_SCOKET=null;
var LANG_MENU=smenu({id:'lang-menu', style:S_SELECT});
for(var l in SupportedLocale) {LANG_MENU.addItem({style:S_TOGGLE|S_LABEL,state:APP_LANG===l,label:l,action:changeLocale})}
function changeLocale() {
    if(APP_ROOT) APP_ROOT.setLocale(SupportedLocale[LANG_MENU.selectedIndex()]); 
    APP_PREF.setCookie('lang',SupportedLocale[LANG_MENU.selectedIndex()]);
};

function uiprop(i) {
    var prop={};
    if (i.I) prop.id=i.I;
    if (i.N) prop.name=i.N;
    if (i.E) prop.element=i.E;
    if (i.L) {
        if (i.L==='HF') prop.layout=sflow(HORIZONTAL);
        else if (i.L==='VF') prop.layout=sflow(VERTICAL);
        else if (i.L==='HC') prop.layout=scard(HORIZONTAL);
        else if (i.L==='VC') prop.layout=scard(VERTICAL);
        else if (i.L==='B') prop.layout=sborder();
        else if (i.L==='G'&&i.G) prop.layout=sgrid(i.G);
    }
    if (i.A!=undefined&&i.A!=null) prop.available=i.A;
    if (i.a) prop.action=i.a;
    if (i.e) prop.event=i.e;
    if (i.c&&0<i.c.length) prop.class=i.c;
    if (i.l) prop.locale=i.l;
    if (i.s) prop.style=i.s;
    if (i.x) prop.x=i.x;
    if (i.y) prop.y=i.y;
    if (i.z) prop.z=i.z;
    if (i.w) prop.width=i.w;
    if (i.h) prop.height=i.h;
    
    return prop;
};
function makeUI() {
    var uis=[];
    for(var u=0;u<APP_UI.uis.length;u++) {
        const info=APP_UI.uis[u];
        const cls=info.C;
        info.prop=uiprop(info);
        if (cls==='E') uis.push(suic(info.prop));
        else if (cls==='V') uis.push(sview(info.prop));
        else if (cls==='P') uis.push(spanel(info.prop));
        //else if (cls==='Fr') uis.push(spanel(info.prop));
        //else if (cls==='W') uis.push(spanel(info.prop));
        
        else if (cls==='Bx') uis.push(sbox(info.Ti,info.prop));
        else if (cls==='ExV') uis.push(sexview(info.prop));
        else if (cls==='TbV') ui=new STabView(info.prop);
        else if (cls==='Li') uis.push(slist(info.prop));
        else if (cls==='LV') uis.push(sliview(info.prop));
        else if (cls==='CV') uis.push(scardview(info.prop));
    
        else if (cls==='Im') uis.push(simgview(info.prop));
        else if (cls==='Wb') uis.push(swebview(info.prop));
    
        else if (cls==='Mi') uis.push(smenuitem(info.prop));
        else if (cls==='M') uis.push(smenu(info.prop));
        else if (cls==='Mb') uis.push(smenubar(info.prop));

        else if (cls==='Fr') uis.push(sform({method:info.M,action:info.V}));
        else if (cls==='I') uis.push(sicon(info.V,info.prop));
        else if (cls==='L') uis.push(slabel(info.V,info.prop));
        else if (cls==='LL') uis.push(slink(info.T,info.V,info.prop));
        else if (cls==='In') {
            if(info.V) info.prop.value=info.V;
            uis.push(sinput(info.S,info.prop));
        }
        else if (cls==='TF') uis.push(stextfield(info.Ti,info.V,info.prop));
    
        else if (cls==='B') {
            if(info.T) info.prop.label=info.T;
            uis.push(sbutton(info.prop));
        }
        else if (cls==='CB') uis.push(scheck(info.prop));
        else if (cls==='RB') uis.push(sradio(info.prop));
        else if (cls==='SEL') uis.push(sselect(info.prop));
        else if (cls==='SL') uis.push(sslider(info.prop));
        else if (cls==='FL') uis.push(new SFileLoader(info.prop));
        else if (cls==='DP') ui=new SDatePicker(info.prop);
        else if (cls==='CP') ui=new SColorPicker(info.prop);
        
        else if (cls==='Bar') uis.push(sbar(info.D));
        else if (cls==='SP') uis.push(sspace());
    }
    arrangeUI(uis);
    return uis;
};
function arrangeUI(u) {
    for(var i=0;i<APP_UI.arrange.length;i++) {
        const a=APP_UI.arrange[i]
        for(var j=1;j<a.length;j++) {
            const c=a[j];
            u[0].add(u[c.i],c.l);
        }
    }
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
    APP_INSTANCE=this;
    this.uis = makeUI();
    APP_ROOT = this.uis[0];
    document.body.appendChild(APP_ROOT.node);
    APP_ABOUT = makeAboutPane(APP_INFO);
    this.prefPane=null;
};
SApp.prototype={
    showAbout: function() {
        if(APP_ABOUT) APP_ABOUT.show();
    },
    showPref: function() {
        this.prefPane.show(true);
    },
    setLang: function(l) {
        APP_LANG = l;
        if (APP_ROOT) APP_ROOT.setLocale(APP_LANG);
    },
    setThema: function(t) {
        document.getElementById('main-thema').href=t;
    }
};