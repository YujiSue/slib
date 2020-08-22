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
    uis:[
        {C:'V',I:'root',N:'root-view',L:'B',l:APP_LANG},
        {C:'Mb',I:'menubar1'},
        {C:'Mi',I:'menuitem01',s:S_ICON,i:'icon:home'},
        {C:'TB',I:'toolbar1'},
        {C:'TI',I:'toolbaritem01',i:'',t:'tool1'},
        {C:'TI',I:'toolbaritem02',i:'',t:'tool2'},
        {C:'TI',I:'toolbaritem03',i:'',t:'tool3'},
        {C:'L',V:'label',I:'label01'},
        {C:'LL',V:'label',I:'label01'},

        {C:'B',T:'Button',I:'button01',a:event01},
    ],
    arrange:[
        [0,{i:1,l:'north'},{i:3,l:'north'}],
        [1,{i:2}],
        [3,{i:4},{i:5},{i:6}]
    ]
};
function event01(e){APP_INSTANCE.showAbout();};

var TestAppLocale = {
    label:{en:'Label',ja:'ラベル'},
    link:{en:'Link',ja:'リンク'},
    label:{en:'Button',ja:'ボタン'},
}
LOCAL_LABEL = Object.assign(LOCAL_LABEL, TestAppLocale);

function TestApp() {
    SApp.call(this);
};
TestApp.prototype=Object.create(SApp.prototype,{});
TestApp.prototype.constructor=TestApp;