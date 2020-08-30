var APP_INFO = {
    name: 'Test',
    icon: 'images/logo.png',
    creator: 'Yuji Suehiro',
    version: '0.1.1',
    develop: '2019/12/21',
    site: 'https://github.com/YujiSue',
    license: 'MIT license.'
};
var APP_UI = {
    elements: {
        root: {UI:'V',N:'root-view',L:'B',l:SAPP_LANG},
        menubar1:{UI:'Mb'},
        menu01:{UI:'M'},
        menu02:{UI:'M'},
        menu03:{UI:'M'},
        menu04:{UI:'M'},
        menuitem01:{UI:'Mi',s:S_ICON,i:'icon:menu',t:'menu',X:BOTTOM,S:'menu01'},
        menuitem02:{UI:'Mi',s:S_LABEL,t:'file',X:BOTTOM,S:'menu02'},
        menuitem03:{UI:'Mi',s:S_LABEL,t:'edit',X:BOTTOM,S:'menu03'},
        menuitem04:{UI:'Mi',t:'about-app',},
        menuitem05:{UI:'Mi',t:'preference',X:RIGHT,S:'menu04'},
        menuitem06:{UI:'Mi',t:'lang',X:RIGHT,S:LANG_MENU},
        menuitem07:{UI:'Mi',t:'thema',X:RIGHT},
        menuitem08:{UI:'Mi',t:'new',k:{mask:DEFAULT_CMD_MASK,key:'n'}},
        menuitem09:{UI:'Mi',t:'open',k:{mask:DEFAULT_CMD_MASK,key:'o'}},
        menuitem10:{UI:'Mi',t:'close',k:{mask:DEFAULT_CMD_MASK,key:'o'}},

        toolbar1:{UI:'TB'},
        toolitem01:{UI:'TI',i:'icon:home',t:'tool1',a:event04},
        toolitem02:{UI:'TI',i:'',t:'tool2'},
        toolitem03:{UI:'TI',i:'',t:'tool3'},

        tabview01:{UI:'TbV'},

        label01:{UI:'L',T:'label'},
        label02:{UI:'L',T:'colored-label',c:['red']},
        label03:{UI:'L',T:'colored-label',c:['blue']},
        label04:{UI:'L',T:'colored-label',c:['green']},
        label05:{UI:'L',T:'unsel-label',c:['unselectable']},
        label06:{UI:'L',T:'event-label',e:[{type:'click',func:event04}]},
        link01:{UI:'LL',T:'link',V:'javascript:void(0);',e:[{type:'click',func:event02}]},
        button01:{UI:'B',t:'button',a:event01},
        button02:{UI:'B',s:S_ICON,i:'icon:image'},
        button03:{UI:'B',s:S_TOGGLE},
        button04:{UI:'B',s:S_TOGGLE},
        field01:{UI:'TF',p:'text'},
    },
    arrange: {
        root:{north:['menubar1','toolbar1'],center:['label01','link01','button01','button02','field01']},
        menubar1:['menuitem01','menuitem02','menuitem03'],
        menu01:['menuitem04','menuitem05'],
        menu02:['menuitem08','menuitem09','menuitem10'],
        //menu03:[],
        menu04:['menuitem06','menuitem07'],
        //toolbar1:['toolitem01','toolitem02','toolitem03'],

    },
};
function event01(e){SAPP_INSTANCE.showAbout();};
function event02(e){SAPP_CONNECT.get({
    url:'./api',
    type:'json',
    next:function(data) {instanceOfID('field01').setValue(data.msg);}
});};
function event03(e){SAPP_CONNECT.get({
    url:'./api',
    type:'json',
    next:function(data) {instanceOfID('field01').setValue(data.msg);}
});};
function event04(e){SAPP_CONNECT.post({
    url:'./api',
    data:{message:'test'}
});};
var TestAppLocale = {
    label:{en:'Label',ja:'ラベル'},
    link:{en:'Link',ja:'リンク'},
    button:{en:'Button',ja:'ボタン'},
}
LOCAL_LABEL = Object.assign(LOCAL_LABEL, TestAppLocale);

function TestApp() {
    SApp.call(this);
};
TestApp.prototype=Object.create(SApp.prototype,{});
TestApp.prototype.constructor=TestApp;