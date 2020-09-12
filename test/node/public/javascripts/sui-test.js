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
        menuitem01:{UI:'Mi',s:S_ICON,I:'icon:menu',T:'menu',o:BOTTOM,M:'menu01'},
        menuitem02:{UI:'Mi',s:S_LABEL,T:'file',o:BOTTOM,M:'menu02'},
        menuitem03:{UI:'Mi',s:S_LABEL,T:'edit',o:BOTTOM,M:'menu03'},
        menuitem04:{UI:'Mi',T:'about-app',a:event01},
        menuitem05:{UI:'Mi',T:'preference',o:RIGHT,M:'menu04'},
        menuitem06:{UI:'Mi',T:'lang',o:RIGHT,M:LANG_MENU},
        menuitem07:{UI:'Mi',T:'thema',o:RIGHT},
        menuitem08:{UI:'Mi',T:'new',k:{mask:DEFAULT_CMD_MASK,key:'n'},a:event02},
        menuitem09:{UI:'Mi',T:'open',k:{mask:DEFAULT_CMD_MASK,key:'o'}},
        menuitem10:{UI:'Mi',T:'close',k:{mask:DEFAULT_CMD_MASK,key:'o'}},

        toolbar1:{UI:'TB'},
        toolitem01:{UI:'TI',I:'icon:home',T:'tool1',a:event04},
        toolitem02:{UI:'TI',I:'',T:'tool2'},
        toolitem03:{UI:'TI',I:'',T:'tool3'},

        label01:{UI:'L',T:'label'},
        label02:{UI:'L',T:'colored-label',c:['red']},
        label03:{UI:'L',T:'colored-label',c:['blue']},
        label04:{UI:'L',T:'colored-label',c:['green']},
        label05:{UI:'L',T:'unsel-label',c:['unselectable']},
        label06:{UI:'L',T:'event-label',e:[{type:'click',func:event04}]},
        link01:{UI:'LL',T:'link',V:'javascript:void(0);',e:[{type:'click',func:event02}]},
        button01:{UI:'B',T:'button'},
        button02:{UI:'B',s:S_ICON,I:'icon:image'},
        button03:{UI:'B',s:S_TOGGLE},
        button04:{UI:'B',s:S_TOGGLE},
        field01:{UI:'TF',p:'text'},

        view01:{UI:'V',c:['main-view']},
        view02:{UI:'V'},
        view03:{UI:'V'},
        view04:{UI:'V'},
        view05:{ui:'V',c:['ui-info']},
        tabview01:{ui:'TbV',c:['main-tabs']},
        treeview01:{ui:'TrV',c:['ui-list']},
        treenode001:{ui:'Tr'},


        panel01:{ui:'P',c:['SUIPane']},
        panel02:{ui:'P',c:['SUIPane']},
        panel03:{ui:'P',c:['SUIPane']},
        panel04:{ui:'P',c:['SUIPane']},


    },
    arrange: {
        root:{north:['menubar01','toolbar01'],center:['view01'],west:['view02'],east:['view03'],south:['view04']},
        view01:['tabview01'],
        view02:['treeview01'],

        //root:{north:['menubar1','toolbar1'],center:['label01','link01','button01','button02','field01']},
        menubar1:['menuitem01','menuitem02','menuitem03'],
        menu01:['menuitem04','menuitem05'],
        menu02:['menuitem08','menuitem09','menuitem10'],
        //menu03:[],
        menu04:['menuitem06','menuitem07'],
        //toolbar1:['toolitem01','toolitem02','toolitem03'],

    },
};
function event01(e){SAPP_INSTANCE.showAbout();};
function event02(e){};
function event05(e){SAPP_CONNECT.get({
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