var APP_INFO = {
    name: 'Test',
    icon: './img/logo.png',
    creator: 'Yuji Suehiro',
    version: '0.1.1',
    develop: '2019/12/21',
    site: 'https://github.com/YujiSue',
    license: 'MIT license.'
};
SAPP_MAIN = sview({id:'app-main',layout:sborder(),class:['auto']});
var TestAppLocale = {
    container:{en:'Container',ja:'コンテナ'},
    view:{en:'View',ja:'ビュー'},
    panel:{en:'Panel',ja:'パネル'},
    box:{en:'Box',ja:'ボックス'},
    exview:{en:'ExpandView',ja:'展開ビュー'},

    string:{en:'Sentence',ja:'文章'},
    label:{en:'Label',ja:'ラベル'},
    link:{en:'Link',ja:'リンク'},
    text:{en:'Text',ja:'テキスト'},

    button:{en:'Button',ja:'ボタン'},
    check:{en:'Check',ja:'チェックボックス'},
    radio:{en:'Radio',ja:'ラジオボタン'},
    

    control:{en:'Control',ja:'操作'},

    abcdefghij:{en:'abcdefghij',ja:'あいうえおかきくけこ'},
}
LOCAL_LABEL = Object.assign(LOCAL_LABEL, TestAppLocale);

var appCenter=sview();
var codeView=stextview({class:['align-center'],color:'white',background:{color:'black'}});

var appMenu=smenu({items:[ABOUT_MENU_ITEM,sbar(HORIZONTAL),PREF_MENU_ITEM]});
var appMenuItem=smenuitem({style:S_ICON,icon:'icon:menu',label:'menu',expand:BOTTOM,submenu:appMenu});
var fileMenuItem=smenuitem({style:S_LABEL,label:'file',expand:BOTTOM});
var editMenuItem=smenuitem({style:S_LABEL,label:'edit',expand:BOTTOM});
var helpMenuItem=smenuitem({style:S_LABEL,label:'help',expand:BOTTOM});
var langMenuItem=smenuitem({style:S_ICON,icon:'icon:language',expand:BOTTOM,submenu:LANG_MENU});
var notifyMenuItem=smenuitem({style:S_ICON,icon:'icon:notifications',expand:BOTTOM});
var mainMenuBar=smenubar({id:'main-menu',items:[appMenuItem,fileMenuItem,editMenuItem,helpMenuItem,sspace(),langMenuItem,SAPP_TIME,notifyMenuItem]});

var homeTool=stoolitem({style:S_ICON,icon:'icon:home'});
var treeTool=stoolitem({style:S_ICON,mode:S_TOGGLE,state:true,icon:'icon:account_tree',action:showWest});
var infoTool=stoolitem({style:S_ICON,mode:S_TOGGLE,state:true,icon:'icon:fact_check',action:showEast}); 
var codeTool=stoolitem({style:S_ICON,mode:S_TOGGLE,state:true,icon:'icon:code',action:showSouth}); 
var mainToolBar=stoolbar({id:'main-tool',items:[homeTool,treeTool,infoTool,codeTool]});

var labelSample=slistview({padding:[10,10,10,10],items:[
    slabel('abcdefghij',{class:['item'],event:[{type:'click',func:showInfo}]}),
    slabel('abcdefghij',{class:['red','item'],event:[{type:'click',func:showInfo}]}),
    slabel('abcdefghij',{class:['orange','item'],event:[{type:'click',func:showInfo}]}),
    slabel('abcdefghij',{class:['yellow','item'],event:[{type:'click',func:showInfo}]}),
    slabel('abcdefghij',{class:['green','item'],event:[{type:'click',func:showInfo}]}),
    slabel('abcdefghij',{class:['blue','item'],event:[{type:'click',func:showInfo}]}),
    slabel('abcdefghij',{class:['navy','item'],event:[{type:'click',func:showInfo}]}),
    slabel('abcdefghij',{class:['purple','item'],event:[{type:'click',func:showInfo}]}),
    slabel('abcdefghij',{class:['bold','item'],event:[{type:'click',func:showInfo}]}),
    slabel('abcdefghij',{class:['italic','item'],event:[{type:'click',func:showInfo}]}),
    slabel('abcdefghij',{class:['gray','item','unselectable'],event:[{type:'click',func:showInfo}]}),
]});
var sviewNode=streenode({label:'view'});
var spaneNode=streenode({label:'panel'});
var sboxNode=streenode({label:'box'});
var sexviewNode=streenode({label:'exview'});
var viewNode=streenode({label:'container',leaf:false,children:[sviewNode,spaneNode,sboxNode,sexviewNode]});


var slabelNode=streenode({label:'label',action:function(e){appCenter.clear();appCenter.add(labelSample);}});
var slinkNode=streenode({label:'link'});
var stextNode=streenode({label:'text'});

var strNode=streenode({label:'string',leaf:false,children:[slabelNode,slinkNode,stextNode]});

var btnNode=streenode({label:'button'});
var checkNode=streenode({label:'check'});
var radioNode=streenode({label:'radio'});

var ctrlNode=streenode({label:'control',leaf:false,children:[btnNode,checkNode,radioNode]});

var uiNode=streenode({leaf:false,state:true,children:[viewNode,strNode,ctrlNode]});
var mainTreeView=streeview({id:'main-tree',root:uiNode,width:'20vw'});

function showWest() {SAPP_MAIN.layout.west.setVisible(treeTool.state);}
function showEast() {SAPP_MAIN.layout.east.setVisible(infoTool.state);}
function showSouth() {SAPP_MAIN.layout.south.setVisible(codeTool.state);}
function showInfo(ui) {

}
var loader=sfileloader();
window.addEventListener('mouseover', function(e){
    if(e.target&&e.target.sui) {
        if(e.target.sui===SAPP_ROOT) codeView.setValue("SAPP_ROOT=sview({element:document.body,id:'app-root',locale:SAPP_LANG,layout:sflow(VERTICAL)});");
        else if(e.target.sui===SAPP_MAIN) codeView.setValue("SAPP_MAIN = sview({id:'app-main',layout:sborder(),class:['auto']});");
        else if(e.target.sui===mainMenuBar) codeView.setValue("var mainMenuBar=smenubar({id:'main-menu',items:[appMenuItem,fileMenuItem,editMenuItem,helpMenuItem,sspace(),SAPP_TIME,notifyMenuItem]});");
        else if(e.target.sui===mainToolBar) codeView.setValue("var mainToolBar=stoolbar({id:'main-tool',items:[homeTool,treeTool,infoTool,codeTool]});");
        else if(e.target.sui===mainTreeView) codeView.setValue("var mainTreeView=streeview({id:'main-tree',root:uiNode,width:'20vw'});");
        else if(e.target.sui===appCenter) codeView.setValue("var appCenter=sview();");
        else if(e.target.sui===appMenu) codeView.setValue("var appMenu=smenu({items:[ABOUT_MENU_ITEM,sbar(HORIZONTAL),PREF_MENU_ITEM]});");
        else if(e.target.sui===homeTool) codeView.setValue("var homeTool=stoolitem({style:S_ICON,icon:'icon:home'});");
        else if(e.target.sui===treeTool) codeView.setValue("var treeTool=stoolitem({style:S_ICON,mode:S_TOGGLE,state:true,icon:'icon:account_tree',action:showWest});");
        else if(e.target.sui===infoTool) codeView.setValue("var infoTool=stoolitem({style:S_ICON,mode:S_TOGGLE,state:true,icon:'icon:fact_check',action:showEast}); ");
        else if(e.target.sui===codeTool) codeView.setValue("var codeTool=stoolitem({style:S_ICON,mode:S_TOGGLE,state:true,icon:'icon:code',action:showSouth}); ");
        

        




        else if(e.target.sui===codeView) codeView.setValue("var codeView=stextview({class:['align-center'],color:'white',background:{color:'black'}});");

        else codeView.setValue('');
    }
    else codeView.setValue('');
});

function TestApp() {
    SAPP_MAIN.add(mainMenuBar,'north').add(mainToolBar,'north').add(mainTreeView,'west').add(appCenter,'center').add(codeView,'south');
    SApp.call(this);
};
TestApp.prototype=Object.create(SApp.prototype,{});
TestApp.prototype.constructor=TestApp;