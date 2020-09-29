const DATE_MODE=0x00;
const TIME_MODE=0x01;

var SAPP_INSTANCE=null;
var SAPP_USER=null;
var SAPP_ROOT=null;
var SAPP_MAIN=null;
var SAPP_AJAX=sajax();
var SAPP_SOCKET=null;

var SAPP_PREF=new SCookie();
var SAPP_PREF_PANE=null;
var PREF_MENU_ITEM=smenuitem({label:'preference',action:showPrefPane});
function makePref() {
    SAPP_PREF_PANE = sdialog({
        modal: true,
        class: ['app-pref'],
        components:[
            sform({

            }),
            spane({
                layout: sflow(HORIZONTAL),
                components:[
                    sspace(),
                    sbutton({label: 'import', class: ['pref-import']}),
                    sbutton({label: 'export', class: ['pref-export']}),
                    sspace(),
                    sbutton({label: 'cancel', class: ['pref-cancel'], action: function() { SAPP_PREF_PANE.hide(); }}),
                    sbutton({label: 'save', class: ['pref-save'], action: function() {form.submit(); SAPP_PREF_PANE.hide();}}),


                ]
            })
        ]
    });
};
function showPrefPane() {if(SAPP_PREF_PANE){SAPP_PREF_PANE.show();}};
var SAPP_THEMA=SAPP_PREF.at('thema')?SAPP_PREF.at('thema'):'/stylesheets/sui-default.css';
function changeThema(t) {
    SAPP_THEMA = t;
    document.getElementById('main-thema').href=SAPP_THEMA;
};
var SAPP_LANG=SAPP_PREF.at('lang')?SAPP_PREF.at('lang'):'ja';
var LANG_MENU=smenu({id:'lang-menu',expand:RIGHT,mode:'select',selected:SupportedLocale.indexOf(SAPP_LANG)});
for(var l=0;l<SupportedLocale.length;l++) {LANG_MENU.addItem({style:S_TOGGLE|S_LABEL,state:SAPP_LANG===SupportedLocale[l],label:SupportedLocale[l],action:changeLocale})}
function changeLocale() {
    SAPP_LANG=SupportedLocale[LANG_MENU.selectedIndex()];
    if(SAPP_ROOT) SAPP_ROOT.setLocale(SAPP_LANG);
    SAPP_PREF.set('lang',SAPP_LANG);
};
var SAPP_ABOUT=null;
var ABOUT_MENU_ITEM = smenuitem({label:'about-app',action:showAbout});
function makeAbout() {
    SAPP_ABOUT = sdialog({
        mode:'generic',
        content:spanel({
            class:['app-about'],
            layout:sflow(VERTICAL),
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
        })
    });
}
function showAbout() {if(SAPP_ABOUT){SAPP_ABOUT.show();}};
var SAPP_MENU_ITEM=smenuitem({style:S_ICON,icon:'icon:menu',items:[ABOUT_MENU_ITEM,sbar(),PREF_MENU_ITEM]});
var SAPP_TIME_MODE=TIME_MODE;
var SAPP_TIMER=stimer(1000,updateTime,2);
var SAPP_TIME=slabel('',{id:'app-time',class:['unselectable'],event:[{type:'click',func:changeTimeMode}]});
var SAPP_TIME_FUNC=[];
function changeTimeMode(){SAPP_TIME_MODE=SAPP_TIME_MODE===TIME_MODE?DATE_MODE:TIME_MODE;};
function updateTime(){
    SAPP_TIME.setText(SAPP_TIME_MODE===TIME_MODE?(new Date()).toLocaleTimeString():(new Date()).toLocaleDateString());
    if(SAPP_TIME_FUNC.length) {for(var i=0;i<SAPP_TIME_FUNC.length;i++){SAPP_TIME_FUNC[i]();}}
};

function SApp() {
    SAPP_INSTANCE=this;
    SAPP_TIMER.start();
    makeAbout();
    //makePref();
    SAPP_ROOT=sview({element:document.body,id:'app-root',locale:SAPP_LANG,layout:sflow(VERTICAL)});
    SAPP_ROOT.add(SAPP_ABOUT);//.add(SAPP_PREF_PANE);
    if(SAPP_MAIN) SAPP_ROOT.add(SAPP_MAIN);
    SAPP_SOCKET = io();
    SAPP_SOCKET.on('notify',function(msg) {
        console.log(msg);
    });
    SAPP_SOCKET.emit('sender','socket test');
};
SApp.prototype={};