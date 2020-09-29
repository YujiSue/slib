var APP_INFO = {
    name: 'Test',
//    type: '',
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
    liview:{en:'ListView',ja:'リストビュー'},
    dialog:{en:'Dialog',ja:'ダイアログ'},

    string:{en:'Sentence',ja:'文章'},
    label:{en:'Label',ja:'ラベル'},
    link:{en:'Link',ja:'リンク'},
    text:{en:'Text',ja:'テキスト'},

    list:{en:'List',ja:'リスト'},
    table:{en:'Table',ja:'テーブル'},
    display:{en:'Display',ja:'表示'},

    button:{en:'Button',ja:'ボタン'},
    check:{en:'Check',ja:'チェックボックス'},
    radio1:{en:'Radio1',ja:'ラジオボタン１'},
    radio2:{en:'Radio2',ja:'ラジオボタン２'},
    
    thumbnail:{en:'Thumbnail',ja:'サムネイル'},
    control:{en:'Control',ja:'操作'},

    abcdefghij:{en:'abcdefghij',ja:'あいうえおかきくけこ'},
}
LOCAL_LABEL = Object.assign(LOCAL_LABEL, TestAppLocale);

var appCenter=sview();
var appMenu=smenu({items:[ABOUT_MENU_ITEM,sbar(HORIZONTAL),PREF_MENU_ITEM]});
var appMenuItem=smenuitem({style:S_ICON,icon:'icon:menu',label:'menu',expand:BOTTOM,submenu:appMenu});
var newMenuItem=smenuitem({label:'new',shortcut:{mask:DEFAULT_CMD_MASK,key:'n'},expand:BOTTOM});

var fileMenu=smenu({items:[newMenuItem]});
var fileMenuItem=smenuitem({style:S_LABEL,label:'file',expand:BOTTOM,submenu:fileMenu});
//var editMenuItem=smenuitem({style:S_LABEL,label:'edit',expand:BOTTOM});
var helpMenuItem=smenuitem({style:S_LABEL,label:'help',expand:BOTTOM});
var langMenuItem=smenuitem({style:S_ICON,icon:'icon:translate',expand:BOTTOM,submenu:LANG_MENU});
var notifyMenuItem=smenuitem({style:S_ICON,icon:'icon:notifications',expand:BOTTOM});
var mainMenuBar=smenubar({items:[appMenuItem,fileMenuItem,helpMenuItem,sspace(),langMenuItem,SAPP_TIME,notifyMenuItem]});
var homeTool=stoolitem({style:S_ICON,icon:'icon:home'});
var treeTool=stoolitem({style:S_ICON,mode:'toggle',state:true,icon:'icon:account_tree',action:showWest});
var infoTool=stoolitem({style:S_ICON,mode:'toggle',state:true,icon:'icon:fact_check',action:showEast}); 
var codeTool=stoolitem({style:S_ICON,mode:'toggle',state:true,icon:'icon:code',action:showSouth});

var smallMenuItem=smenuitem({style:S_LABEL,label:'small'});
var largeMenuItem=smenuitem({style:S_LABEL,label:'large'});
var sizeMenu=smenu({items:[smallMenuItem,largeMenuItem]});
var charsizeTool=stoolitem({style:S_ICON,mode:'select',icon:'icon:text_fields',submenu:sizeMenu}); 

var mainToolBar=stoolbar({items:[homeTool,treeTool,infoTool,codeTool,{type:'bar'},charsizeTool]});
var sviewNode=streenode({label:'view',action:function(e){appCenter.clear();appCenter.add(viewSample);}});
var spaneNode=streenode({label:'panel',action:function(e){appCenter.clear();appCenter.add(panelSample);}});
var sboxNode=streenode({label:'box',action:function(e){appCenter.clear();appCenter.add(boxSample);}});
var sexviewNode=streenode({label:'exview',action:function(e){appCenter.clear();appCenter.add(exviewSample);}});
var viewNode=streenode({label:'container',leaf:false,children:[sviewNode,spaneNode,sboxNode,sexviewNode]});

var listNode=streenode({label:'list',action:function(e){appCenter.clear();appCenter.add(listSample);}});
var tblNode=streenode({label:'table',action:function(e){appCenter.clear();appCenter.add(tableSample);}});
var dispNode=streenode({label:'display',leaf:false,children:[listNode,tblNode]});

var slabelNode=streenode({label:'label',action:function(e){appCenter.clear();appCenter.add(labelSample);}});
var slinkNode=streenode({label:'link',action:function(e){appCenter.clear();appCenter.add(linkSample);}});
var stextNode=streenode({label:'text',action:function(e){appCenter.clear();appCenter.add(textSample);}});

var strNode=streenode({label:'string',leaf:false,children:[slabelNode,slinkNode,stextNode]});

var btnNode=streenode({label:'button',action:function(e){appCenter.clear();appCenter.add(buttonSample);}});

var ctrlNode=streenode({label:'control',leaf:false,children:[btnNode]});

var uiNode=streenode({leaf:false,state:true,children:[viewNode,dispNode,strNode,ctrlNode]});
var mainTreeView=streeview({id:'main-tree',root:uiNode,width:'20vw'});



var codeView=stextview({size:'1.5rem',padding:['2.5%','2.5%','2.5%','2.5%'],minh:'20vh',color:'white',background:{color:'black'}});
var viewSample=slistview({width:'80%',height:'80%',padding:[10,10,10,10],
    components:[
        sview({
            layout:sborder(),
            border:{style:'solid',width:1,color:'black'},
            class:['item','halign-center'],
            components:{
                north:[slabel('north',{size:24})],
                west:[slabel('west',{size:24})],
                east:[slabel('east',{size:24})],
                center:[slabel('center',{size:24})],
                south:[slabel('south',{size:24})]
            },
            event:[{type:'click',func:showInfo}]
        }),
    ]
});
;
var panelSample=scardview({width:'80%',height:'80%',padding:[10,10,10,10],items:[
    spanel({width:'40%',height:'30%',margin:[10,10,10,10],border:{style:'solid',width:2,color:'black'},class:['item'],event:[{type:'click',func:showInfo}]}),
    spanel({width:'40%',height:'30%',margin:[10,10,10,10],border:{style:'solid',width:2,color:'black',radius:10},class:['item'],event:[{type:'click',func:showInfo}]}),
    spanel({width:'40%',height:'30%',margin:[10,10,10,10],border:{style:'double',width:3,color:'black',radius:10},class:['item'],event:[{type:'click',func:showInfo}]}),
    spanel({width:'40%',height:'30%',margin:[10,10,10,10],border:{style:'dashed',width:2,color:'black',radius:10},class:['item'],event:[{type:'click',func:showInfo}]}),
]});
var boxSample=sview({width:'80%',height:'80%',padding:[10,10,10,10],components:[
    sbox({width:300,height:200,label:'box',class:['item'],content:stextview({editable:true,value:'Box Content'})})
]});
var exviewSample=sview({width:'80%',height:'80%',padding:[10,10,10,10],components:[
    sexview({border:{style:'solid',width:1,color:'black'},class:['item'],content:stextview({value:'ExpandView Content'})}),
    sexview({style:ORIGINAL,border:{style:'solid',width:1,color:'black'},class:['item'],content:stextview({value:'ExpandView Content'}),icon:['icon:add_circle_outline','icon:remove_circle_outline']})
]});
var listSample=sview({width:'80%',height:'80%',padding:[10,10,10,10],components:[
    slist({border:{style:'solid',width:1,color:'black'},class:['item'],items:[{content:'list1'},{content:'list2'},{content:'list3'}]}),
    slist({prefix:'lower-alpha',border:{style:'solid',width:1,color:'black'},class:['item'],items:[{content:'list1'},{content:'list2'},{content:'list3'}]}),
    slist({type:'ul',prefix:'disc',border:{style:'solid',width:1,color:'black'},class:['item'],items:[{content:'list1'},{content:'list2'},{content:'list3'}]}),
]});
function alertEvent(e) {alert('Fired.');}
var tableSample=sview({width:'80%',height:'80%',padding:[10,10,10,10],components:[
    stable({showRowHeader:true,
        columns:[{label:'index'},{label:'name'},{label:'adult',renderer:new SCheckCellRenderer()},{label:'sex',renderer:new SSelectCellRenderer(['male','female','none'])},{label:'action',renderer:new SButtonCellRenderer('Fire!')}],
        rows:[[1,'John Smith',true,'male',alertEvent],[2,'Jane Doe',false,'female',alertEvent]]
    })
]});
var labelSample=slistview({width:'80%',height:'80%',padding:[10,10,10,10],items:[
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
var linkSample=slistview({width:'80%',height:'80%',padding:[10,10,10,10],items:[
    slink('link',null,{class:['item'],event:[{type:'click',func:showInfo}]}),
    slink('google','https://www.google.com',{newtab:true,class:['item'],event:[{type:'click',func:showInfo}]}),
    slink('thumbnail','https://yujisue.github.io/',{newtab:true,class:['item'],
        event:[
            {type:'click',func:showInfo},
            {type:'mouseover',func:showThumbnail1},
            {type:'mouseleave',func:hideThumbnail1},
        ]
    }),
]});
var thumb1=spanel({position:'absolute',z:1,border:{style:'solid',width:1,color:'black'},components:[swebview({src:'https://yujisue.github.io/',width:320,height:240})]});
function showThumbnail1(e){SAPP_ROOT.add(thumb1);thumb1.setX(e.clientX).setY(e.clientY);};
function hideThumbnail1(e){SAPP_ROOT.remove(thumb1);};
var textSample=slistview({width:'80%',height:'80%',padding:[10,10,10,10],items:[
    stextview({value:'Twas brillig, and the slithy toves<br>Did gyre and gimble in the wabe;<br>All mimsy were the borogoves,<br>And the mome raths outgrabe.',border:{style:'solid',width:1,color:'black'},class:['item'],event:[{type:'click',func:showInfo}]}),
    stextview({value:'於是天神、諸命以、<br>詔伊邪那岐命・伊邪那美命二柱神<br>「修理固成是多陀用幣流之國。」<br>賜天沼矛而言依賜也。<br>故、二柱神、立天浮橋而指下其沼矛以畫者、<br>鹽許々袁々呂々邇畫鳴而引上時、<br>自其矛末垂落之鹽累積、<br>成嶋、是淤能碁呂嶋。',editable:true,border:{style:'solid',width:1,color:'black'},class:['item'],event:[{type:'click',func:showInfo}]}),
]});
var buttonSample=scardview({width:'80%',height:'80%',padding:[10,10,10,10],items:[
    sbutton({label:'button',class:['item'],event:[{type:'click',func:showInfo}]}),
    sbutton({mode:'push',width:20,height:20,border:{radius:11},class:['item'],event:[{type:'click',func:showInfo}]}),
    sbutton({mode:'toggle',size:40,class:['item'],event:[{type:'click',func:showInfo}]}),
    sbutton({mode:'panel',content:spanel({width:50,height:50,background:{color:'radial-gradient(white,silver)'}}),class:['item'],event:[{type:'click',func:showInfo}]}),
    scheck({label:'check'}),
    sradio({label:'radio1',name:'radio'}),
    sradio({label:'radio2',name:'radio'}),
]});



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
        

        




        else if(e.target.sui===codeView) codeView.setValue("var codeView = stextview({ size: '1.5rem', padding: ['2.5%','2.5%','2.5%','2.5%'], minh: '20vh', color: 'white', background: {color:'black'} });");

        else codeView.setValue('');
    }
    else codeView.setValue('');
});
function TestApp() {
    SAPP_MAIN.add(mainMenuBar,'north')
    .add(mainToolBar,'north')
    .add(mainTreeView,'west')
    .add(appCenter,'center')
    .add(codeView,'south');
    SApp.call(this);
};
TestApp.prototype=Object.create(SApp.prototype,{});
TestApp.prototype.constructor=TestApp;