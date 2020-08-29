var PC_DEVICE=0x01;
var MOBILE_DEVICE=0x02;
var WINDOWS_DEVICE=0x10;
var APPLE_DEVICE=0x20;
var LINUX_DEVICE=0x40;
var UNIX_DEVICE=0x80;
var CLIENT_ENV=0;
var OS='';
if (typeof platform!=undefined){
    OS=platform.os;
    var os_str=platform.os.toString().toLowerCase();
    if(os_str.indexOf("windows") !== -1) CLIENT_ENV=PC_DEVICE|WINDOWS_DEVICE;
    else if(os_str.indexOf("mac") !== -1) CLIENT_ENV=PC_DEVICE|APPLE_DEVICE;
    else if(os_str.indexOf("android") !== -1) CLIENT_ENV=MOBILE_DEVICE|LINUX_DEVICE;
    else if(os_str.indexOf("ios") !== -1) CLIENT_ENV=MOBILE_DEVICE|APPLE_DEVICE;    
}
var _parser=new DOMParser();
function codedString(s){
    return _parser.parseFromString(s,"text/html").body.innerText;
};
var SHIFT_MASK=0x01;
var CTRL_MASK=0x02;
var ALT_MASK=0x04;
var CMD_MASK=0x08;
var DEFAULT_CMD_MASK=0x10;
var SPACE_CHAR=codedString('&nbsp;');
var AMP_CHAR=codedString('&amp;');
var QUOT_CHAR=codedString('&quot;');
var DEGREE_CHAR=codedString('&deg;');
var DOT_CHAR=codedString('&middot;');
var CIRC_CHAR=codedString('&circ;');
var TILDA_CHAR=codedString('&tilde;');
var LT_CHAR=codedString('&lt;');
var GT_CHAR=codedString('&gt;');
var CHECK_CHAR=codedString('&#10003;');
var POST_CHAR=codedString('&#12306;');
var COPYWRITE_CHAR=codedString('&copy;');
var TRI_CHARS=[codedString('&#9650;'),codedString('&#9660;'),codedString('&#9664;'),codedString('&#9654;')];
var CROSS_CHAR=codedString('&#215;');
var COMMAND_CHAR=codedString('&#8984;');
var CTRL_CHAR=codedString('&#8963;');
var SHIFT_CHAR=codedString('&#8679;');
var OPT_CHAR=codedString('&#8997;');

var DEFAULT=0x0000;
var ORIGINAL=0x0001;
var S_ICON=0x0001;
var S_LABEL=0x0002;
var S_BUTTON=0x0004;
var S_SELECT=0x0008;
var S_TOGGLE=0x0010;
var S_PANEL=0x0020;
var S_MODIFIER=0x0040;

//For Dialog
var ALERT_DIALOG=0x01;
var CONFIRM_DIALOG=0x02;
var PROMPT_DIALOG=0x04;
var GENERIC_DIALOG=0x08;
var MODAL_FLAG=0x10;
//Tree
var NORMAL_TREE=0x01;
var FILE_TREE=0x02;
//DIRECTION
var VERTICAL=0x01;
var HORIZONTAL=0x02;
var DEPTH=0x04;
var DIVERGENCE=0x11;
var CONVERGENCE=0x12;
var CW=0x21;
var CCW=0x22;
//POSITION
var TOP=0x00;
var BOTTOM=0x01;
var LEFT=0x02;
var RIGHT=0x03;
var NONE=0xFF;

var CURRENT_MENU=null;

if(!String.prototype.startsWith){String.prototype.startsWith=function(s,p){return this.substr((p||0),s.length)===s;};}
if(!String.prototype.endsWith){String.prototype.endsWith=function(s,p){return this.substr(this.length-s.length-(p||0),s.length)===s;};}
function propOverride(p,o){
    if(!p) p=o; 
    else for(var k in o){if(Object.keys(p).indexOf(k)==-1) p[k]=o[k];}
    return p;
};
function encodeForm(d){
    var params=[];
    for(var k in d){
        params.push(encodeURIComponent(k)+'='+encodeURIComponent(d[k]));
    }
    return params.join('&').replace(/%20/g,'+');
}
async function useHash(t,f) {
    const e=new TextEncoder().encode(t);
    const d=await crypto.subtle.digest('SHA-256',e);
    const a=Array.from(new Uint8Array(d));
    const s=a.map(b=>b.toString(16).padStart(2,'0')).join('');
    if(f) f(s);
}
function instanceOfID(i,s){if(!s){s=document;} const e=s.getElementById(i); if(e){return e.sui;} else return null;};
function inRange(x,y,s){
    if(s) return x>=s.X()&&x<=(s.X()+s.width())&&y>=s.Y()&&y<=s.Y()+s.height();
    return false;
};
function menuRoot(sui){
    
}
function parseIcon(s,c) {
    if (s.startsWith('icon:')) return sicon(s.substring(5),{class:['unselectable','unresponsible']});
    else if(s.startsWith('image:')) return simgview({src:s.substring(6),class:['unselectable','unresponsible']});
    else return slabel(s,{class:['unselectable','unresponsible']});
}
function SSocket(n) {
    this.socket=io();
    this.connectID=n;
    this.response=null;
    const Cls=this;
    this.socket.on(Cls.connectID,function(d) {
        if (Cls.response) Cls.response(d);
    });
};
SSocket.prototype={
    send: function(d){this.socket.emit(this.connectID,d);},
    setResponse:function(r){this.response=r;}
};
function SConnect() {};
SConnect.prototype={
    get: async function(p) {
        p=propOverride(p,{url:'',type:'',next:null,error:null});
        const xhr=new XMLHttpRequest();
        xhr.open('GET',p.url);
        xhr.responseType=p.type;
        xhr.send();
        xhr.onreadystatechange=function() {
            if (xhr.readyState===XMLHttpRequest.DONE) {
                if (xhr.status===200 && p.next) p.next(xhr.response);
                else if(p.error) p.error(xhr.status);
            }
        }
    },
    post: async function(p) {
        p=propOverride(p,{url:'',data:null,next:null,error:null});
        var xhr=new XMLHttpRequest();
        xhr.open('POST',p.url);
        xhr.setRequestHeader('Content-Type', 'application/x-www-form-urlencoded');
        xhr.send(encodeForm(p.data));
        xhr.onreadystatechange=function() {
            if (this.readyState===XMLHttpRequest.DONE) {
                if (xhr.status===200&&p.next) p.next(xhr.response);
                else if(p.error) p.error(xhr.status);
            }
        }
    }
};
function SGET(u,t) {
    return new Promise(function(resolve,reject) {
        var xhr=new XMLHttpRequest();
        xhr.open('GET',u);
        xhr.responseType=t;
        xhr.send();
        xhr.onreadystatechange=function() {
            if (xhr.readyState===XMLHttpRequest.DONE) {
                if (xhr.status===200) resolve(xhr.response);
                else reject();
            }
        }
    });
};
function SPOST(u,t,d) {
    return new Promise(function(resolve,reject) {
        var xhr=new XMLHttpRequest();
        xhr.open('POST',u);
        xhr.setRequestHeader('Content-Type',t);
        xhr.send(d);
        xhr.onreadystatechange=function() {
            if (this.readyState===XMLHttpRequest.DONE) {
                if (xhr.status===200) resolve(xhr.response);
                else reject(xhr.status);
            }
        }
    });
};
function SDownloader(n,b) {
    const u=URL.createObjectURL(b);
    const a=document.createElement("a");
    document.body.appendChild(a);
    a.download=n;
    a.href=u;
    a.click();
    a.remove();
    URL.revokeObjectURL(u);
};
function SCookie(p) {
    p=propOverride(p,{samesite:'lax',secure:true,path:null,age:null,expires:null});
    if(p.samesite) document.cookie='samesite='+p.samesite;
    if(p.secure) document.cookie='secure';
    if(p.path) document.cookie='path='+p.path;
    if(p.expires) document.cookie='expires='+p.expires;
    else if(p.age) document.cookie='max-age='+p.age;
};
SCookie.prototype={
    setCookie: function(k,v) {document.cookie=k+'='+v;},
    cookieAt: function(k) {
        var c=document.cookie.split('; ');
        for(i=0;i<c.length;i++){
            if(c[i].startsWith(k+'=')) return c[i].split('=')[1];
        }
        return null;
    },
    cookies:function() {
        var d={};
        var c=document.cookie.split('; ');
        for(i=0;i<c.length;i++){
            if(c[i].indexOf('=')!=-1) {
                const v= c[i].split('=');
                d[v[0]]=v[1];
            }
        }
        return d;
    },
    setExpireSec: function(s) {document.cookie='max-age='+s;},
    setExpire:function(d) {document.cookie='expires='+d},
    clear:function(){document.cookie='max-age=-1';}
};
function SShortCut(s,a){
    this.key=s.key;
    this.mask=s.mask;
    if (this.mask&DEFAULT_CMD_MASK){
        if (CLIENT_ENV&APPLE_DEVICE) this.mask=this.mask-DEFAULT_CMD_MASK+CMD_MASK;
        else this.mask=this.mask-DEFAULT_CMD_MASK+CTRL_MASK;
    }
    this.action=a;
};
SShortCut.prototype={
    toString: function(){
        var str='';
        if (CLIENT_ENV&APPLE_DEVICE){
            if(this.mask&CMD_MASK) str+=COMMAND_CHAR;
            if(this.mask&CTRL_MASK) str+=CTRL_CHAR;
            if(this.mask&SHIFT_MASK) str+=SHIFT_CHAR;
            if(this.mask&ALT_MASK) str+=OPT_CHAR;
        }
        else {
            if(this.mask&CTRL_MASK) str+='Ctrl+';
            if(this.mask&SHIFT_MASK) str+='Shift+';
            if(this.mask&ALT_MASK) str+='Alt+';
        }
        str+=this.key.toUpperCase();
        return str;
    }
};
function SShortCutManager(){this.sclist={};};
SShortCutManager.prototype={
    register: function(s){
        if (this.sclist[s.key]) this.sclist[s.key][s.mask]=s.action;
        else {
            this.sclist[s.key]=new Array(16);
            this.sclist[s.key][s.mask]=s.action;
        }
    },
    fire: function(e){
        if (this.sclist[SKEY_STATE.key]&&
            this.sclist[SKEY_STATE.key][SKEY_STATE.mask]){
                this.sclist[SKEY_STATE.key][SKEY_STATE.mask](e);
            }
    }
};
function addSKeyEventListener(t,a){SKEY_EVENTLISTENER[t].pushed(a);}
var SKEY_EVENTLISTENER={
    down:[],
    pressed:[],
    up:[]
};
var SC_MANAGER=new SShortCutManager();
var SKEY_STATE={
    source: null,
    pushed: false,
    mask: 0,
    key: ''
};
window.addEventListener('keydown', function(e){
    if (e.target&&e.target.sui) SKEY_STATE.source=e.target.sui;
    SKEY_STATE.pushed=true;
    if (e.altKey) SKEY_STATE.mask|=ALT_MASK;
    if (e.shiftKey) SKEY_STATE.mask|=SHIFT_MASK;
    if (e.ctrlKey ) SKEY_STATE.mask|=CTRL_MASK;
    if (e.metaKey ) SKEY_STATE.mask|=CMD_MASK;
    if(SKEY_EVENTLISTENER.down.length) {
        for(var a in SKEY_EVENTLISTENER.down) {a(e);}
    }
});
window.addEventListener('keypress', function(e){
    SC_MANAGER.fire(e);
    if(SKEY_EVENTLISTENER.pressed.length) {
        for(var a in SKEY_EVENTLISTENER.pressed) {a(e);}
    }
});
window.addEventListener('keyup', function(e){
    if(SKEY_EVENTLISTENER.up.length) {
        for(var a in SKEY_EVENTLISTENER.up) {a(e);}
    }
    SKEY_STATE.source=null;
    SKEY_STATE.pushed=false;
    SKEY_STATE.mask=0;
    SKEY_STATE.key='';
});
var SMOUSE_EVENTLISTENER={
    down:[],
    over:[],
    enter:[],
    leave:[],
    up:[],
    click:[]
};
var SMOUSE_STATE={
    source: undefined,
    count: 0,
    button: 0,
    pushed: false,
    dragging: false,
    oriX: -1,
    oriY: -1,
    diffX: -1,
    diffY: -1
};
document.addEventListener('mousedown',function(e){
    if (e.target&&e.target.sui) SMOUSE_STATE.source=e.target.sui;
    SMOUSE_STATE.pushed=true;
    SMOUSE_STATE.dragging=false;
    SMOUSE_STATE.oriX=e.clientX;
    SMOUSE_STATE.oriY=e.clientY;
});
document.addEventListener('mousemove',function(e){
    if(SMOUSE_STATE.pushed) {
        SMOUSE_STATE.dragging=true;
        SMOUSE_STATE.diffX=e.clientX-SMOUSE_STATE.oriX;
        SMOUSE_STATE.diffY=e.clientY-SMOUSE_STATE.oriY;
        SMOUSE_STATE.oriX=e.clientX;
        SMOUSE_STATE.oriY=e.clientY;
        if(SMOUSE_STATE.source.suiid==='splitter') SMOUSE_STATE.source.update(e);
        //console.log(SMOUSE_STATE.diffX+","+SMOUSE_STATE.diffY);
    }
    else {
        if (e.target&&e.target.sui) SMOUSE_STATE.source=e.target.sui;
    }
});
document.addEventListener('mouseup',function(e){
    SMOUSE_STATE.source=null;
    SMOUSE_STATE.pushed=false;
    SMOUSE_STATE.dragging=false;
    SMOUSE_STATE.oriX=e.clientX;
    SMOUSE_STATE.oriY=e.clientY;
});
document.addEventListener('mouseover',function(e){
    //console.log(event.target.sui);
});
document.addEventListener('click',function(e){
    if(e.target&&e.target.sui) SMOUSE_STATE.source=e.target.sui;
    if(e.target) {
        if(e.target.sui) {
            if(CURRENT_MENU) {
                if (e.target.sui != CURRENT_MENU) CURRENT_MENU.close();
            }
        }
        else {
            if(CURRENT_MENU) CURRENT_MENU.close();
        }
    }
    else {
        if(CURRENT_MENU) CURRENT_MENU.close();
    }
});
document.addEventListener('touchstart',function(e){
    if(e.target&&e.target.sui) SMOUSE_STATE.source=e.target.sui;
    if(e.target) {
        if(e.target.sui) {
            if(CURRENT_MENU) {
                if (e.target.sui != CURRENT_MENU) CURRENT_MENU.close();
            }
        }
        else {
            if(CURRENT_MENU) { CURRENT_MENU.close(); }
        }
    }
    else {
        if(CURRENT_MENU) { CURRENT_MENU.close(); }
    }
});
document.addEventListener('drag',function(e) {
});
document.addEventListener('dragstart',function(e) {
    if (e.target&&e.target.sui) SMOUSE_STATE.source=e.target.sui;
    SMOUSE_STATE.dragging=true;
});
document.addEventListener('dragend', function(e) {

});
document.addEventListener('dragover', function(e) {
    
});
document.addEventListener('dragenter', function(e) {
    
});
document.addEventListener('dragleave', function(e) {
    
});
document.addEventListener('drop',function(e) {
    event.preventDefault();
    
});


function slayout(a){return new SLayout(a);};
function scolumn(a){return new SColumnLayout(a);};
function sflow(a,b){return new SFlowLayout(a,b);};
function scard(a,b){return new SCardLayout(a,b);};
function sborder(){return new SBorderLayout();};
function sgrid(a,b,c){return new SGridLayout(a,b,c);};
function suic(a,b){return new SUIComponent(a,b);};
function sspace(){return new SSpacer();};
function sbar(a,b){return new SBar(a,b);};
function sview(a){return new SView(a);}
function spanel(a){return new SPanel(a);}
function sdialog(a){return new SDialog(a);}
function sbox(a,b){return new SBox(a,b);}
function sexview(a){return new SExpandView(a);}
function sframe(a){return new SFrame(a);}
function stabview(a){return new STabView(a);}

function ssplitter(a,b,c){return new SSplitter(a,b,c);}
function ssplitview(a,b){return new SSplitView(a,b);}
function simgview(a){return new SImageView(a);}
function swebview(a){return new SWebView(a);}
function slistitem(a){return new SListItem(a);};
function slist(a){return new SList(a);};
function slistview(a){return new SListView(a);};
function scardview(a){return new SCardView(a);};
function stablecolumn(a,b){return new STableColumn(a,b);};
function stablerow(a,b){return new STableRow(a,b);};
function stablecell(a,b,c,d){return new STableCell(a,b,c,d);};
function stableview(a){return new STableView(a);};
function stable(a){return new STable(a);};
function streenode(a){return new STreeNode(a);};
function streeview(a){return new STreeView(a);};

function smenuitem(a){return new SMenuItem(a);}
function smenu(a){return new SMenu(a);}
function smenubar(a){return new SMenuBar(a);}
function stoolitem(a){return new SToolItem(a);};
function stoolbar(a){return new SToolBar(a);};

function sform(a){return new SForm(a);};
function sinput(a,b){return new SInput(a,b);};
function stextfield(a){return new STextField(a);};
function stextarea(a){return new STextArea(a);};
function slabel(a,b){return new SLabel(a,b);};
function slink(a,b,c){return new SLinkLabel(a,b,c);};
function sicon(a,b){return new SIcon(a,b);};
function sbutton(a){return new SButton(a);};
function sradio(a){return new SRadioButton(a);};
function scheck(a){return new SCheckBox(a);};
function sselitem(a){return new SSelectItem(a);};
function sselect(a){return new SSelector(a);};
function sslider(a){return new SSlider(a);};
function sfileio(a){return new SFileLoader(a);};
function sprogress(a){return new SProgress(a);};
function stimer(a,b,c){return new STimer(a,b,c);};

function SLayout(t){this.type=t;this.C=undefined;};
SLayout.prototype={
    applyLayout:function(c){this.C=c;this.C.setDisplay(this.type);},
    append:function(c,l){this.C.node.appendChild(c);},
    insert:function(i,c,l){this.C.node.insertBefore(c,this.C.node.childNodes[i]);},
    remove:function(c,l){c.parentNode.removeChild(c);},
    clear:function(l){while(this.C.node.hasChildNodes()){this.C.node.removeChild(this.C.node.childNodes[0]);}}
};
function SColumnLayout(n){SLayout.call(this,'flex');this.count=n;};
SColumnLayout.prototype=Object.create(SLayout.prototype,{
    applyLayout:{value:function(c){
        SLayout.prototype.applyLayout.apply(this,[c]);
        this.C.addClass('column-layout');
        this.C.node.style.columnCount=this.count;
   }}
});
SColumnLayout.prototype.constructor=SColumnLayout;
function SFlowLayout(d,r){SLayout.call(this,'flex');this.direction=d;this.reverse=(r==null?false:r);};
SFlowLayout.prototype=Object.create(SLayout.prototype,{
    applyLayout:{value:function(c){
        SLayout.prototype.applyLayout.apply(this,[c]);
        this.C.addClass('flow-layout');
        this.C.addClass(this.direction==VERTICAL?'vertical':'horizontal');
        if(this.reverse) this.C.addClass('reverse');
   }}
});
SFlowLayout.prototype.constructor=SFlowLayout;
function SCardLayout(d,r){SLayout.call(this,'flex'); this.direction=d;this.reverse=(r==null?false:r);};
SCardLayout.prototype=Object.create(SLayout.prototype,{
    applyLayout:{value:function(c){
        SLayout.prototype.applyLayout.apply(this,[c]);
        this.C.addClass('card-layout');
        this.C.addClass(this.direction==VERTICAL?'vertical':'horizontal');
        if(this.reverse) this.C.addClass('reverse');
   }}
});
SCardLayout.prototype.constructor=SCardLayout;
function SBorderLayout(){SLayout.call(this,'flex');};
SBorderLayout.prototype=Object.create(SLayout.prototype,{
    applyLayout:{value:function(c){
        SLayout.prototype.applyLayout.apply(this,[c]);
        this.C.addClass('border-layout');
        this.north=sview({class:['north']});
        this.med=sview({layout:SFlowLayout(HORIZONTAL),class:['med']});
        this.west=sview({class:['west']});
        this.center=sview({class:['center']});
        this.east=sview({class:['east']});
        this.south=sview({class:['south']});
        this.med.add(this.west).add(this.center).add(this.east);
        this.C.node.appendChild(this.north.node);
        this.C.node.appendChild(this.med.node);
        this.C.node.appendChild(this.south.node);
   }},
    append:{value:function(c,l){
        if(l==='north') this.north.node.appendChild(c);
        else if(l==='south') this.south.node.appendChild(c);
        else if(l==='west') this.west.node.appendChild(c);
        else if(l==='east') this.east.node.appendChild(c);
        else if(l==='center') this.center.node.appendChild(c);
   }},
    clear:{value:function(l){
            while ( this.north.node.node.hasChildNodes() ){
                this.north.node.node.removeChild( this.north.node.node.childNodes[0] );
           }
            while ( this.south.node.node.hasChildNodes() ){
                this.south.node.node.removeChild( this.south.node.node.childNodes[0] );
           }
            while ( this.west.node.node.hasChildNodes() ){
                this.west.node.node.removeChild( this.west.node.node.childNodes[0] );
           }
            while ( this.east.node.node.hasChildNodes() ){
                this.east.node.node.removeChild( this.east.node.node.childNodes[0] );
           }
            while ( this.center.node.node.hasChildNodes() ){
                this.center.node.node.removeChild( this.center.node.node.childNodes[0] );
           }
       }
   }
});
SBorderLayout.prototype.constructor=SBorderLayout;
function SGridLayout(r,c,a){SLayout.call(this,'grid');
    this.rows=[];
    this.columns=[];
    this.areas=[]; 
    for(var i=0;i<r.length;i++){
        if(typeof(r[i])==='number'){
            if(r[i]===0) this.rows.push('auto');
            else if(r[i]<0) this.rows.push((-r[i])+'fr');
            else this.rows.push(r[i]+'px');
        }
        else this.rows.push(r[i]);
    };
    for (var j=0;j<c.length;j++){
        if(typeof(c[j])==='number'){
            if(c[j]=== 0) this.columns.push('auto');
            else if(c[j]<0) this.columns.push((-c[j])+'fr');
            else this.columns.push(c[j]+'px');
        }
        else this.columns.push(c[j]);
    };
    if(a===null){
        this.areas=Array(this.rows.length).fill(Array(this.columns.length).fill('area'));
        var k=1;
        for (var i=0;i<this.rows.length;i++){
            for (var j=0;j<this.columns.length;j++){
                this.areas[i][j]=this.areas[r][c]+k; k++;
           }
       }
    }
    else this.areas=a;
};
SGridLayout.prototype=Object.create(SLayout.prototype,{
    applyLayout:{
        value:function(c){
            SLayout.prototype.applyLayout.apply(this,[c]);
            this.C.addClass('grid-layout');
            var tmp='';
            for(var r=0;r<this.rows.length;r++){tmp+=this.rows[r]+' ';};
            this.C.node.style.gridTemplateRows=tmp;
            tmp='';
            for (var c=0;c<this.columns.length;c++){tmp+=this.columns[c]+' ';};
            this.C.node.style.gridTemplateColumns=tmp;
            tmp='';
            for (var a=0;a<this.areas.length;a++){
                var tmp2='"'+this.areas[a][0];
                for (var a2=1;a2<this.areas[a].length;a2++) {tmp2+=' '+this.areas[a][a2];} tmp2+='"';
                tmp+=tmp2+' ';
            };
            this.C.node.style.gridTemplateAreas=tmp;
        }
    },
    append:{value:function(c,l){this.C.node.appendChild(c);c.style.gridArea=l;}}
});
SGridLayout.prototype.constructor=SGridLayout;
function SUIComponent(t,p){
    p=propOverride(p,{element:null});
    this.node=document.createElement(t);
    this.node.sui=this;
    this.suiid=null;
    this.layout=null;
    this.parent=null;
    this.components=[];
    this.display=null;
    this.locale=null;
    this.draggable=false;
    this.initNode(p);
    if(p.element) p.element.appendChild(this.node);
};
SUIComponent.prototype={
    initNode:function(p){
        this.setDisplay(p.display);
        this.node.classList.add('component');
        if(p.id) this.setID(p.id);
        if(p.name != undefined) this.setName(p.name);
        if(p.tooltip != undefined) this.setToolTip(p.tooltip);
        if(p.uiid != undefined) this.setSuiID(p.uiid);
        if(p.locale != undefined) this.setLocale(p.locale);
        if(p.class != undefined && 0 < p.class.length) this.setClass(p.class);
        if(p.layout != undefined) this.setLayout(p.layout);
        if(p.components) this.setComponents(p.components);
        if(p.margin != undefined && 0 < p.margin.length){this.setMargin(p.margin);}
        if(p.padding != undefined && 0 < p.padding.length){this.setPadding(p.padding);}
        if(p.halign != undefined) this.setHorizontalAlign(p.halign);
        if(p.valign != undefined) this.setVerticalAlign(p.valign);
        if(p.cursor != undefined) this.setCursor(p.cursor);
        if(p.x != undefined) this.setX(p.x);
        if(p.y != undefined) this.setY(p.y);
        if(p.z != undefined) this.setZ(p.z);
        if(p.position != undefined) this.setPosition(p.position);
        if(p.width != undefined) this.setWidth(p.width);
        if(p.height != undefined) this.setHeight(p.height);
        if(p.minw != undefined) this.setMinWidth(p.minw);
        if(p.maxw != undefined) this.setMaxWidth(p.maxw);
        if(p.minh != undefined) this.setMinHeight(p.minh);
        if(p.maxh != undefined) this.setMaxHeight(p.maxh);
        if(p.size != undefined) this.setSize(p.size);
        if(p.draggable != undefined) this.setDraggable(p.draggable);
        if(p.border != undefined) this.setBorder(p.border);
        if(p.background != undefined) this.setBackground(p.background);
        if(p.shadow != undefined) this.setShadow(p.shadow);
        if(p.color != undefined) this.setColor(p.color);
        if(p.event != undefined) this.setEventListener(p.event);
        if(p.available != undefined) this.setAvailable(p.available);
        if(p.visible != undefined) this.setVisible(p.visible);
    },
    setDisplay:function(d){this.display=d; if(d) this.node.style.display=d; return this;},
    setLocale:function(l){this.locale=l; for(var i=0;i<this.components.length;i++){this.components[i].setLocale(l);} return this;},
    isLocalized :function(){return this.locale;},
    root:function(){
        var p=this.parent;
        if(!p) return this;
        while (p.parent){p=p.parent;}
        return p;
    },
    id:function(){return this.node.id;},
    setID:function(i){this.node.id=i; return this;},
    name:function(){return this.node.name;},
    setName:function(n){this.node.name=n; return this;},
    tooltip:function(){return this.node.title;},
    setToolTip:function(t){this.node.title=t; return this;},
    suiID:function(){return this.suiid;},
    setSuiID:function(i){this.suiid=i; return this;},
    setVisible:function(v){
        if(v) this.node.style.display=this.display;
        else if(this.node.style.display!='none'){
            this.display=this.node.style.display;
            this.node.style.display='none';
        }
        return this;
    },
    setAttribute(a){for(var k in a){this.node.k=a.k;} return this;},
    setAvailable:function(a){this.node.disabled=!a; return this;},
    setMainClass:function(c){this.node.classList.replace(this.node.classList[0],c); return this;},
    setClass:function(c){for (var i=0;i<c.length;i++){this.addClass(c[i]);} return this;},
    hasClass:function(c){return this.node.classList.contains(c);},
    addClass:function(c){this.node.classList.add(c); return this;},
    removeClass:function(c){if(this.node.classList.contains(c)) this.node.classList.remove(c); return this;},
    componentAt:function(i){return this.components[i];},
    setComponents:function(c){
        if(c instanceof Array&&c.length) for(var i=0;i<c.length;i++){this.add(c[i]);}
        else {for(var e in c) {for(var i=0;i<c[e].length;i++){this.add(c[e][i],e);}}}
        return this;
    },
    elementCount:function(){return this.components.length;},
    add:function(ui,loc){
        if(loc == null) loc=undefined;
        ui.setParent(this);
        if(this.locale != undefined) ui.setLocale(this.locale);
        this.components.push(ui);
        if(this.layout != undefined) this.layout.append(ui.node,loc);
        else this.node.appendChild(ui.node);
        return this;
    },
    insert:function(idx,ui,loc){
        if(loc == null) loc=undefined;
        ui.setParent(this);
        if(this.locale != undefined) ui.setLocale(this.locale);
        this.components.splice(idx,0,ui);
        if(this.layout != undefined){
            /*
            
            */
            this.layout.append(idx,ui.node,loc);
       }
        else this.node.insertBefore(ui.node,this.components[idx].node);
    },
    remove:function(ui){
        ui.setParent(undefined);
        ui.node.parentNode.removeChild(ui.node);
        this.components.splice(this.components.indexOf(ui),1);
    },
    setParent:function(element){this.parent=element; return this;},
    setLayout:function(layout){
        this.layout=layout;
        if(this.layout != undefined){
            this.layout.applyLayout(this);
       }
        return this;
    },
    setMargin:function(margin){
        this.node.style.marginTop=margin[0]+'px'; 
        this.node.style.marginRight=margin[1]+'px'; 
        this.node.style.marginBottom=margin[2]+'px'; 
        this.node.style.marginLeft=margin[3]+'px'; 
    },
    setMarginT:function(margin){this.node.style.marginTop=margin;},
    setMarginB:function(margin){this.node.style.marginBottom=margin;},
    setMarginL:function(margin){this.node.style.marginLeft=margin;},
    setMarginR:function(margin){this.node.style.marginRight=margin;},
    setPadding:function(padding){
        this.node.style.paddingTop=padding[0]+'px'; 
        this.node.style.paddingRight=padding[1]+'px'; 
        this.node.style.paddingBottom=padding[2]+'px'; 
        this.node.style.paddingLeft=padding[3]+'px'; 
    },
    setHorizontalAlign:function(align){this.node.style.textAlign=align; return this;},
    setVerticalAlign:function(align){ this.node.style.verticalAlign=align; return this;},
    setCursor:function(cursor){this.node.style.cursor=cursor; return this;},
    setX:function(x){
        if(typeof(x)==='number') this.node.style.left=x+'px';
        else this.node.style.left=x;
        return this;
    },
    setXr:function(x){
        if(typeof(x)==='number') this.node.style.right=x+'px';
        else this.node.style.right=x;
        return this;
    },
    X:function(){return this.node.getBoundingClientRect().left+window.pageXOffset;},
    setY:function(y){
        if(typeof(y)==='number') this.node.style.top=y+'px';
        else this.node.style.top=y;
        return this;
    },
    setYr:function(y){
        if(typeof(y)==='number') this.node.style.bottom=y+'px'; 
        else this.node.style.bottom=y;
        return this;
    },
    Y:function(){return this.node.getBoundingClientRect().top+window.pageYOffset;},
    setZ:function(z){this.node.style.zIndex=z; return this;},
    setPosition:function(pos){this.node.style.position=pos; return this;},
    setWidth:function(w){
        if(typeof(w) == 'number') this.node.style.width=w + 'px'; 
        else this.node.style.width=w;
        return this;
    },
    setHeight:function(h){
        if(typeof(h) == 'number') this.node.style.height=h + 'px'; 
        else this.node.style.height=h;
        return this;
    },
    setMinWidth:function(w){
        if(typeof(w) == 'number') this.node.style.minWidth=w + 'px'; 
        else this.node.style.minWidth=w;
        return this;
    },
    setSize:function(size){this.node.style.fontSize=size + 'px';},
    setDraggable:function(d){this.node.draggable=d;},
    setBorder:function(border){
        if(border.style != undefined) this.node.style.borderStyle=border.style;
        if(border.width != undefined) this.node.style.borderWidth=border.width;
        if(border.color != undefined) this.node.style.borderColor=border.color;
        if(border.radius != undefined) this.node.style.borderRadius=border.radius;
        if(border.top != undefined){
            var tborder=border.top;
            if(tborder.style != undefined) this.node.style.borderTopStyle=tborder.style;
            if(tborder.width != undefined) this.node.style.borderTopWidth=tborder.width;
            if(tborder.color != undefined) this.node.style.borderTopColor=tborder.color;
            if(tborder.radius != undefined) this.node.style.borderTopRadius=tborder.radius;
       }
        if(border.bottom != undefined){
            var bborder=border.bottom;
            if(bborder.style != undefined) this.node.style.borderBottomStyle=bborder.style;
            if(bborder.width != undefined) this.node.style.borderBottomWidth=bborder.width;
            if(bborder.color != undefined) this.node.style.borderBottomColor=bborder.color;
            if(bborder.radius != undefined) this.node.style.borderBottomRadius=bborder.radius;
       }
        if(border.left != undefined){
            var lborder=border.left;
            if(lborder.style != undefined) this.node.style.borderLeftStyle=lborder.style;
            if(lborder.width != undefined) this.node.style.borderLeftWidth=lborder.width;
            if(lborder.color != undefined) this.node.style.borderLeftColor=lborder.color;
            if(lborder.radius != undefined) this.node.style.borderLeftRadius=lborder.radius;
       }
        if(border.right != undefined){
            var rborder=border.right;
            if(rborder.style != undefined) this.node.style.borderRightStyle=rborder.style;
            if(rborder.width != undefined) this.node.style.borderRightidth=rborder.width;
            if(rborder.color != undefined) this.node.style.borderRightColor=rborder.color;
            if(rborder.radius != undefined) this.node.style.borderRightRadius=rborder.radius;
       }
        return this;
   },
    setBackground:function(back){
        if(back.color != undefined){
            this.node.style.background=back.color;
       }
        if(back.image != undefined){
            this.node.style.backgroundImage=back.image;
       }
        return this;
   },
    setShadow:function(shadow){
        this.node.style.boxShadow=shadow; return this;
    },
    setFont:function(font){
        if(font.name != undefined) this.node.style.fontFamily=font.name;
        if(font.style != undefined){
            var style=font.style;
            if(style != undefined && 0 < style.length){
                for (var i=0; i < style.length; i++){
                    var element=style[i];
                    if(element == 'bold'){
                        this.node.style.fontWeight='bold';
                   }
                    else this.node.style.fontStyle=element;
               }
           }
       } 
        if(font.size != undefined) this.node.style.fontSize=font.size;
        return this;
    },
    setColor:function(col){this.node.style.color=col; return this;},
    setEventListener:function(l) {
        for(var i=0;i<l.length;i++) {
            this.node.addEventListener(l[i].type,l[i].func);
        }
        return this;
    },
    width:function(){return this.node.offsetWidth;},
    height:function(){return this.node.offsetHeight;},
    boundary:function(){return [this.X(),this.Y(),this.width(),this.height()];},
    isVisible:function(){return !(this.node.display=='none');},
    clear:function(){
        while (this.node.hasChildNodes()) this.node.removeChild(this.node.lastChild);
        this.components=[]; return this;
    }
};
function SSpacer(){SUIComponent.call(this,'div',{class:['sspacer','auto','unresponsible']});};
SSpacer.prototype=Object.create(SUIComponent.prototype,{
    initNode:{value:function(p){
        SUIComponent.prototype.initNode.apply(this,[p]);
        this.setSuiID('spacer').setMainClass('sspacer');
    }}});
SSpacer.prototype.constructor=SSpacer;
function SBar(d,p){p=propOverride(p,{style:'solid',width:1,color:'dimgray'});
    if(d===HORIZONTAL) SUIComponent.call(this,'div',{border:{top:p}});
    else SUIComponent.call(this,'div',{border:{left:p}});
};
SBar.prototype=Object.create(SUIComponent.prototype,{
    initNode:{value:function(p){
        SUIComponent.prototype.initNode.apply(this,[p]); 
        this.setSuiID('separator').setMainClass('sseparator');
   }},
});
SBar.prototype.constructor=SBar;
function SView(p){p=propOverride(p,{});SUIComponent.call(this,'div',p);};
SView.prototype=Object.create(SUIComponent.prototype, {
    initNode:{value:function(p){
        SUIComponent.prototype.initNode.apply(this,[p]);
        this.setSuiID('view').setMainClass('sview');
    }}
});
SView.prototype.constructor=SView;
function SPanel(p){p=propOverride(p,{});SUIComponent.call(this,'div',p);};
SPanel.prototype=Object.create(SUIComponent.prototype, {
    initNode:{value:function(p) {
        SUIComponent.prototype.initNode.apply(this,[p]); 
        this.setSuiID('panel').setMainClass('spane;');
    }}
});
SPanel.prototype.constructor=SPanel;
function SFrame(p) {
    p=propOverride(p,{rorate:false,resize:true,move:true,header:TOP,hdrag:true,close:true,minimize:true,maximize:true});
    p.layout=sflow(VERTICAL);
    SUIComponent.call(this,'div',p);
};
SFrame.prototype=Object.create(SUIComponent.prototype, {
    initNode:{
        value:function(p) {
            SUIComponent.prototype.initNode.apply(this,[{
                layout:sgrid(
                    [10,5,0,5,10],
                    [10,5,0,5,10],
                    [
                        ['r1','d1','d1','d1','r2'],
                        ['d2','s1','s2','s3','d3'],
                        ['d2','s4','m','s5','d3'],
                        ['d2','s6','s7','s8','d3'],
                        ['r3','d4','d4','d4','r4']
                    ]
                )
            }]); 
            const Cls=this;
            this.current=[0,0];
            this.hdrag=p.hdrag;
            this.min=false;
            this.resizePane=[
                spanel({cursor:'nw-resize',class:['frame-resize']}),
                spanel({cursor:'n-resize',class:['frame-resize']}),
                spanel({cursor:'ne-resize',class:['frame-resize']}),
                spanel({cursor:'w-resize',class:['frame-resize']}),
                spanel({cursor:'e-resize',class:['frame-resize']}),
                spanel({cursor:'sw-resize',class:['frame-resize']}),
                spanel({cursor:'s-resize',class:['frame-resize']}),
                spanel({cursor:'se-resize',class:['frame-resize']})
            ];
            this.dragPane=[
                spanel({cursor:'move',class:['frame-drag']}),
                spanel({cursor:'move',class:['frame-drag']}),
                spanel({cursor:'move',class:['frame-drag']}),
                spanel({cursor:'move',class:['frame-drag']})
            ];
            this.rotPane=[
                spanel({class:['frame-rot','cwrotator']}),
                spanel({class:['frame-rot','cwrotator']}),
                spanel({class:['frame-rot','cwrotator']}),
                spanel({class:['frame-rot','cwrotator']})
            ]
            this.mainPanel=spanel(p);
            this.header=sview({layout:sflow(HORIZONTAL),class:['frame-header']});
            this.header.setDraggable(p.hdrag);
            this.header.node.onmousemove=function(e){
                if(p.hdrag && SMOUSE_STATE.dragging) Cls.move(SMOUSE_STATE.diffX,SMOUSE_STATE.diffY);
            };
            this.content=sview({class:['frame-content']});
            this.closer=sbutton({style:S_PANEL,class:['frame-closer']});
            this.mbuttonPane=spanel({layout:sflow(HORIZONTAL),class:['frame-mbuttons']});
            this.minimizer=sbutton({style:S_PANEL,class:['frame-minimizer'],action:function(e){Cls.minimize();}});
            this.maximizer=sbutton({style:S_PANEL,class:['frame-maximizer']});
            this.mbuttonPane.add(this.minimizer).add(this.maximizer);
            if(p.header===NONE) this.header.setVisible(false);
            else {
                

                if(!p.close) this.closer.setAvailable(false);
                if(!p.minimize) this.minimizer.setAvailable(false);
                if(!p.maximize) this.maximizer.setAvailable(false);
            }
            this.header.add(sspace()).add(this.mbuttonPane).add(this.closer);
            this.mainPanel.addClass('frame-main').add(this.header).add(this.content);
            this.setSuiID('frame')
            .setMainClass('sframe');
            for(var i=0;i<4;i++) this.layout.append(this.rotPane[i].node,'r'+(i+1));
            for(var i=0;i<4;i++) this.layout.append(this.dragPane[i].node,'d'+(i+1));
            for(var i=0;i<8;i++) this.layout.append(this.resizePane[i].node,'s'+(i+1));
            this.layout.append(this.mainPanel.node,'m');
        }
    },
    add:{value:function(u) {this.components.push(u); this.content.add(u); return this;}},
    minimize:{value:function(){if(this.min) this.content.setVisible(true); else this.content.setVisible(false); this.min=!this.min;}},
    move:{value:function(dx,dy){this.setX(this.X()+dx);this.setY(this.Y()+dy);}},
});
SFrame.prototype.constructor=SFrame;
function SDialog(p) {
    p=propOverride(p,{mode:GENERIC_DIALOG,modal:true,message:''})
    this.mode=p.mode;
    this.init(p);            
};
SDialog.prototype={
    init:function(p) {
        this.result=null;
        this.modal=null;
        this.suiid='dialog';
        if (this.mode===GENERIC_DIALOG) {
            const Cls=this;
            this.screen=sview({layout:sflow(VERTICAL),class:['sscreen','align-center']});
            this.frame=spanel(p);
            this.setModal(p.modal);
            this.frame.setMainClass('sdialog');
            this.frame.node.onclick=function(e){e.stopPropagation();};
            this.screen.node.onclick=function(e){Cls.hide();};
            this.screen.add(this.frame).setVisible(false);
        }
        else this.message=p.message;
    },
    add:function(u,l) { this.frame.add(u,l); return this;},
    show:function() {
        switch(this.mode) {
            case ALERT_DIALOG:
                alert(this.message);
                break;
            case CONFIRM_DIALOG:
                this.result=confirm(this.message);
                break;
            case PROMPT_DIALOG:
                this.result=prompt(this.message);
                break;
            default:
                this.screen.setVisible(true);
                this.frame.addClass('show');
                break;
        }
    },
    hide:function(){
        this.screen.setVisible(false);
        this.frame.removeClass('show');
    },
    setModal:function(m){
        this.modal=m;
        if(m) this.screen.addClass('modal');
        else this.screen.removeClass('modal');
        return this;
    }
};
function SBox(t,p){p=propOverride(p,{});this.label=slabel(t);SUIComponent.call(this,'fieldset',p);};
SBox.prototype=Object.create(SUIComponent.prototype, {
    initNode:{
        value:function(p) {
            SUIComponent.prototype.initNode.apply(this,[p]);
            this.legend=suic('legend');
            this.legend.add(this.label);
            this.setSuiID('box')
            .setMainClass('sbox')
            .add(this.legend);
        }
    },
    title:{value:function(){return this.label.text();}},
    setTitle:{value:function(t){this.label.setText(t); return this;}}
});
SBox.prototype.constructor=SBox;
function SExpandView(p) {
    p=propOverride(p,{style:DEFAULT,direction:BOTTOM,expand:false,labels:['show','hide'],icons:[TRI_CHARS[RIGHT],TRI_CHARS[BOTTOM]],content:null});
    this.direction=p.direction;
    if (p.style) {
        if (p.direction<2) p.layout=sflow(VERTICAL);
        else p.layout=sflow(HORIZONTAL);
        SUIComponent.call(this,'div',p);
    }
    else SUIComponent.call(this,'details',p);
};
SExpandView.prototype=Object.create(SUIComponent.prototype, {
    initNode:{
        value:function(p) {
            SUIComponent.prototype.initNode.apply(this,[p]);
            const Cls=this;
            this.setSuiID('exview').setMainClass('sexview');
            this.labels=p.labels;
            this.icons=null;
            this.header=null;
            this.label=slabel('',{class:['sexview-label']});
            this.content=sview({ class:['sexview-content']});
            if (p.style) {
                this.header=sview({layout:sflow(this.direction<2?VERTICAL:HORIZONTAL)});
                this.controler=spanel({class:['sexview-expander']});
                this.icons=[parseIcon(p.icons[0]),parseIcon(p.icons[1])];
                if (this.direction<2) this.controler.addClass('horizontal');
                else {
                    this.label.addClass('vtext');
                    this.controler.addClass('vertical');
                }
                if (this.expand===0||this.expand===2) this.addClass('reverse');
                this.header.add(this.controler).add(this.label);
                this.controler.node.onclick=function(e){Cls.setState(!Cls.state);}
            }
            else {
                this.header=suic('summary');
                this.header.add(this.label);
                this.node.ontoggle=function(e){Cls.state=Cls.node.open; Cls.update();}
            }
            this.add(this.header)
            .add(this.content)
            .setContent(p.content)
            .setState(p.expand);
        }
    },
    expand:{value:function(){this.setState(true); return this;}},
    collapse:{value:function(){this.setState(false); return this;}},
    update:{value:function() {
        if (this.state) this.addClass('open');
        else this.removeClass('open');
        this.label.setText(this.labels[this.state?1:0]);
        if (this.node.tag==='div') {
            this.controler.clear();
            this.controler.add(this.icons[this.state?1:0]);
        }
    }},
    setState:{value:function(s) {
        this.state=s;
        if (this.node.tag==='details') this.node.open=this.s;
        else this.content.setVisible(s);
        this.update(); return this;
    }},
    setContent:{value:function(c) {this.content.clear();if(c) this.content.add(c); return this;}}
});
SExpandView.prototype.constructor=SExpandView;
function STab(p) {
    p=propOverride(p,{direction:HORIZONTAL,content:null,icon:null,label:'tab',close:true,draggable:true});
    p.layout=sflow(p.direction);
    SUIComponent.call(this,'div',p);
}
STab.prototype=Object.create(SUIComponent.prototype, {
    initNode:{
        value:function(p) {
            SUIComponent.prototype.initNode.apply(this,[p]);
            var Cls=this;
            this.active=false;
            this.icon=null;
            if(p.icon) this.icon=parseIcon(p.icon)
            else this.icon=slabel('',{class:['unselectable','unresponsible']});
            this.icon.addClass('tab-icon');
            this.title=slabel(p.label,{class:['tab-title','unselectable']});
            this.node.onclick=function(e){
                Cls.parent.select(Cls);
            };
            this.closer=spanel({class:['tab-closer']});
            this.closer.node.onclick=function(e) {
                e.stoppagation(); 
                Cls.parent.close(Cls); 
            } 
            if(p.close) this.closer.setVisible(false);
            this.setSuiID('tab')
            .setMainClass('stab')
            .add(sspace())
            .add(this.icon)
            .add(this.title)
            .add(sspace())
            .add(this.closer)
            .setContent(p.content);
        }
    },
    setActive:{value:function(a){
        this.active=a;
        if(a) this.addClass('active');
        else this.removeClass('active');
    }},
    setContent:{value:function(c){this.content=c; return this;}},
    title:{value:function(){return this.title.text();}},
    setTitle:{value:function(t){this.title.setText(t); return this;}}
});
STab.prototype.constructor=STab;
function STabViewHeader(d) {
    SUIComponent.call(this,'div',{layout:sflow(d)});
};
STabViewHeader.prototype=Object.create(SUIComponent.prototype, {
    initNode:{
        value:function(p) {
            SUIComponent.prototype.initNode.apply(this,[p]);
            var Cls=this;
            this.setSuiID('tabheader')
            .setMainClass('stab-header');
        }
    },
    addTab:{value:function(t){}},
    removeTab:{value:function(t){}}
});
STabViewHeader.prototype.constructor=STabViewHeader;
function STabView(p) {
    p=propOverride(p,{hpos:TOP,direction:VERTICAL,tabs:[],selected:0});
    p.layout=sflow(p.direction);
    SUIComponent.call(this,'div',p);
};
STabView.prototype=Object.create(SUIComponent.prototype, {
    initNode:{
        value:function(p) {
            SUIComponent.prototype.initNode.apply(this,[p]);
            this.tabs=[];
            this.selected=null;
            this.header=new STabViewHeader(p.direction==HORIZONTAL?VERTICAL:HORIZONTAL);
            this.content=sview({class:['tabview-content']});
            if (p.tabs.length) {
                for (var i=0;i<p.tabs.length;i++) {
                    this.addTab(p.tabs[t]);
                }
            }
            this.setSuiID('tabview')
            .setMainClass('stabview')
            .add(this.header)
            .add(this.content)
            .selectIndex(p.selected);
        }
    },
    addTab:{
        value:function(p) {
            var tab=new STab(p);
            this.tabs.push(tab);
            this.header.addTab(tab);
            return this;
        }
    },
    indexOf:{value:function(t){return this.tabs.indexOf(t);}},
    select:{
        value:function(t) {
            if (this.selected) {
                this.selected.removeClass('active');
                this.selected.active=false;
            }
            this.content.clear();
            this.selected=t;
            this.selected.addClass('active');
            if(t.content) this.content.add(t.content);
            return this;
        }
    },
    selectIndex:{
        value:function(i) {
            if (i<this.tabs.length) this.select(this.tabs[i]);
        }
    },
    close:{value:function(t) {
        const i=this.indexOf(t);
        this.header.removeTab(t);
        this.tabs.splice(i, 1);
        if (t.active) {
            this.content.clear();
            if (i===this.tabs.length) i--;
            this.selectIndex(i);
        }
    }}
});
STabView.prototype.constructor=STabView;

function SSplitter(p,s,d) {
    this.primary=p;
    this.secondary=s;
    this.direction=d;
    this.border=null;
    this.edge=(p==null||s==null);
    SUIComponent.call(this,'div',{});
};
SSplitter.prototype=Object.create(SUIComponent.prototype, {
    initNode:{
        value:function(p) {
            SUIComponent.prototype.initNode.apply(this,[p]);
            const Cls=this;
            this.setSuiID('splitter')
            .setMainClass('ssplitter')
            .addClass(this.direction==HORIZONTAL?'horizontal':'vertical');
        }
    },
    update: {value: function(e){
        if(this.direction==HORIZONTAL) {
            if(this.primary) {
                if(this.primary.width()+SMOUSE_STATE.diffX<0) {
                    this.primary.setWidth(0);
                }
                else this.primary.setWidth(this.primary.width()+SMOUSE_STATE.diffX);
            }
            if(this.secondary) {
                if(this.secondary.X()+SMOUSE_STATE.diffX<this.primary.X()) {
                    this.secondary.setX(this.primary.X());
                }
                else {
                    this.secondary.setX(this.secondary.X()+SMOUSE_STATE.diffX);
                }
            }
        }
        else {
            if(this.primary) {
                if(this.primary.height()+SMOUSE_STATE.diffY<0) {
                    this.primary.setHeight(0);
                }
                else this.primary.setHeight(this.primary.height()+SMOUSE_STATE.diffY);
            }
            if(this.secondary) {
                if(this.secondary.Y()+SMOUSE_STATE.diffY<this.primary.Y()) {
                    this.secondary.setY(this.primary.Y());
                }
                else {
                    this.secondary.setY(this.secondary.Y()+SMOUSE_STATE.diffY);
                }
            }
        }
    }}
});
SSplitter.prototype.constructor=SSplitter;
function SSplitView(d,p) {
    p=propOverride(p,{ratio:1.0,primary:null,secondary:null});
    this.direction=d;
    p.layout=sflow(d);
    SUIComponent.call(this,'div',p);
}
SSplitView.prototype=Object.create(SView.prototype, {
    initNode:{
        value:function(p) {
            SUIComponent.prototype.initNode.apply(this,[p]);
            if(this.direction==HORIZONTAL) this.primary=sview({width:(50.0*p.ratio)+'%',class:['split-primary']});
            else this.primary=sview({height:(50.0*p.ratio)+'%',class:['split-primary']});
            this.secondary=sview({class:['split-secondary']});
            if(p.primary) this.primary.add(p.primary);
            if(p.secondary) this.secondary.add(p.secondary);
            this.splitter=ssplitter(this.primary,this.secondary,this.direction);
            this.setSuiID('splitview')
            .setMainClass('split-view')
            .add(this.primary)
            .add(this.splitter)
            .add(this.secondary);
        }
    },
    setPrimaryView:{value:function(v) {
        this.primary.clear();
        this.primary.add(v);
    }},
    setSecondaryView:{value:function(v) {
        this.secondary.clear();
        this.secondary.add(v);
    }}
});
SSplitView.prototype.constructor=SSplitView;
function SWebView(p) {p=propOverride(p,{src:''});SUIComponent.call(this,'iframe',p);}
SWebView.prototype=Object.create(SUIComponent.prototype, {
    initNode:{
        value:function(p) {
            SUIComponent.prototype.initNode.apply(this, [ p ]);
            this.setSuiID('webview')
            .setMainClass('swebview')
            .setAttribute({frameBorder:'0'})
            .setSource(p.src);
        }
    },
    source:{value:function(){return this.node.contentWindow.location;}},
    setSource:{value:function(src){this.node.src=src; return this;}}
});
SWebView.prototype.constructor=SWebView;
function SImageView(p){p=propOverride(p,{src:'',alt:'',width:null,height:null});SUIComponent.call(this,'img',p);};
SImageView.prototype=Object.create(SUIComponent.prototype, {
    initNode:{
        value:function(p) {
            SUIComponent.prototype.initNode.apply(this,[p]);
            const Cls=this;
            this.oriWidth=0;
            this.oriHeight=0;
            this.image=new Image();
            this.image.onload=function() {
                Cls.oriWidth=Cls.image.width;
                Cls.oriHeight=Cls.image.height;
            };
            this.setSuiID('image')
            .setMainClass('simage')
            .setSource(p.src)
            .setAlt(p.alt)
            .setWidth(p.width)
            .setHeight(p.height);
        }
    },
    source:{value:function(){return this.node.src;}},
    setSource:{value:function(s){
        this.image.src=s;this.node.src=s;return this;
    }},
    altText:{value:function(){return this.node.alt;}},
    setAlt:{value:function(t){this.node.alt=t; return this;}},
    setWidth:{value:function(w){if(w) this.node.width=w; return this;}},
    setHeight:{value:function(h){if(h) this.node.height=h; return this;}}
});
SImageView.prototype.constructor=SImageView;

function SSvgCanvas(p) {

}
function SFigure(p) {
    if (p == null) p={};
    SUIComponent.call(this,'svg',p);
};
SFigure.prototype=Object.create(SUIComponent.prototype, {
    initNode:{
        value:function(p){
            SUIComponent.prototype.initNode.apply(this, [ p ]);
            this.setSuiID('graphic')
            .setMainClass('sgraphic');
        }
    }
});
SFigure.prototype.constructor=SFigure;

function SCanvas(p) {
    p=propOverride(p,{width:640,height:480});
    this.context=null;
    SUIComponent.call(this,'canvas',p);
};
SCanvas.prototype=Object.create(SUIComponent.prototype, {
    initNode:{
        value:function(p){
            SUIComponent.prototype.initNode.apply(this, [ p ]);
            this.setSuiID('drawer')
            .setMainClass('sdrawer')
            .setWidth(p.width)
            .setHeight(p.height);
            this.context=this.node.getContext('2d');
        }
    },
    setWidth:{value:function(w){this.node.width=w; return this;}},
    setHeight:{value:function(h){this.node.height=h; return this;}},
    setPainter:{value:function(p) {
        p=propOverride(p,{fill:null,stroke:null,width:null,alpha:null})
        if(p.fill) this.context.fillStyle=p.fill;
        if(p.stroke) this.context.strokeStyle=p.stroke;
        if(p.witdth) this.context.lineWidth=p.width;
        if(p.alpha) this.context.globalAlpha=p.alpha;
        return this;
    }},
    path:{value:function(x,y){this.context.beginPath();}},
    moveTo:{value:function(x,y){this.context.moveTo(x,y);}},
    lineTo:{value:function(x,y){this.context.lineTo(x,y);}},
    curveTo:{value:function(rx,ry,x,y){this.context.quadraticCurveTo(rx,ry,x,y);}},
    curveTo:{value:function(rx1,ry1,rx2,ry2,x,y){this.context.bezierCurveTo(rx1,ry1,rx2,ry2,x,y);}},
    closePath:{value:function(){this.context.closePath();}},
    drawPath:{value:function(){this.context.stroke();}},
    fillPath:{value:function(){this.context.fill();}},
    arc:{value:function(x,y,r,s,e,d) {this.context.arc(x,y,r,s,e,d);}},
    drawRect: {value:function(x,y,w,h,p) {
        if(p) this.setPainter(p);
        this.context.fillRect(x,y,w,h);
        this.context.strokeRect(x,y,w,h);
    }},
    drawRoundRect: {value:function(x,y,w,h,r){
        this.context.beginPath();
        this.context.moveTo(x,y+r);
        this.context.lineTo(x,y+h-r);
        this.context.arcTo(x,y+h,x+r,y+h,r);
        this.context.lineTo(x+w-r,y+h);
        this.context.arcTo(x + width, y + height, x + width, y + height - radius, radius);
        this.context.lineTo(x + width, y + radius);
        this.context.arcTo(x + width, y, x + width - radius, y, radius);
        this.context.lineTo(x + radius, y);
        this.context.arcTo(x, y, x, y + radius, radius);
        this.context.fill();
        this.context.stroke();
    }},
    drawEllipse: {value:function(x,y,w,h,t,p) {
        if(p) this.setPainter(p);

    }},
    drawArc: {value:function(x,y,w,h,b,e,t,p) {
        if(p) this.setPainter(p);

    }},
    drawPolygon: {value:function(x,y,n,t,p) {
        if(p) this.setPainter(p);

    }},
    erase: {value:function(x,y,w,h) {
        this.context.clearRect(x,y,w,h);
    }}

});
SCanvas.prototype.constructor=SCanvas;

function SListItem(p){p=propOverride(p,{content:null});SUIComponent.call(this,'li',p);};
SListItem.prototype=Object.create(SUIComponent.prototype,{
    initNode:{value:function(p){
        SUIComponent.prototype.initNode.apply(this,[p]);
        this.setSuiID('listitem').setMainClass('sli-item');
        if(p.content) this.node.innerHTML=p.content;
    }}
});
SListItem.prototype.constructor=SListItem;
function SList(p){p=propOverride(p,{type:'ol',prefix:'decimal',items:[]});SUIComponent.call(this,p.type,p);};
SList.prototype=Object.create(SUIComponent.prototype,{
    initNode:{
        value:function(p){
            SUIComponent.prototype.initNode.apply(this,[ p ]);
            this.setSuiID('list').setMainClass('slist');
            this.node.style.listStyle=p.prefix;
            if(p.items.length){
                for (var i in p.items){
                    if(i.suiid) this.add(i);
                    else this.add(slistitem(i)); 
                }
            }
        }
    }
});
SList.prototype.constructor=SList;
function SListView(p){
    p=propOverride(p,{direction:VERTICAL,items:[]});
    p.layout=sflow(p.direction);
    SUIComponent.call(this,'div',p);
};
SListView.prototype=Object.create(SUIComponent.prototype,{
    initNode:{
        value:function(p){
            SUIComponent.prototype.initNode.apply(this,[p]);
            this.setSuiID('listview').setMainClass('slist-view');
            if(p.items.length){for(var i in items){this.add(i);}}
        }
    }
});
SListView.prototype.constructor=SListView;
function SCardView(p){
    p=propOverride(p,{direction:HORIZONTAL,items:[]});
    p.layout=scard(p.direction);
    SUIComponent.call(this,'div',p);
};
SCardView.prototype=Object.create(SUIComponent.prototype,{
    initNode:{
        value:function(p){
            SUIComponent.prototype.initNode.apply(this,[ p ]);
            this.setSuiID('cardview').setMainClass('scard-view');
            if(p.items.length){for(var i in items){this.add(i);}}
        }
    }
});
SCardView.prototype.constructor=SCardView;

var SCellRenderer={
    render:function(c,p) {
        c.add(suic('pre',p));
        if(p.value) c.componentAt(0).node.innerHTML=p.value;
        c.componentAt(0).setMainClass('cell-default');
        c.componentAt(0).node.ondblclick=function(e){
            c.componentAt(0).node.contentEditable=true; 
            c.componentAt(0).addClass('editing');
        };
        c.componentAt(0).node.onblur=function(e){
            c.componentAt(0).node.contentEditable=false; 
            c.componentAt(0).removeClass('editing');
        };
    },
    setValue:function(c,v) {c.componentAt(0).node.innerHTML=v;},
    getValue:function(c) {return c.componentAt(0).node.innerHTML;},
    setEditable:function(c,e) {
        c.componentAt(0).node.contentEditable=e; 
        if(e) c.componentAt(0).addClass('editing');
        else c.componentAt(0).removeClass('editing');
    }
};
var SCheckCellRenderer={
    render:function(c,p) {
        c.add(sinput('checkbox',p));
        c.componentAt(0).setValue(p.value);
        c.componentAt(0).setMainClass('cell-check');
        c.componentAt(0).node.onchange=function(e){
            if(p.action) p.action(e);
        };
    },
    setValue:function(c,v) {c.componentAt(0).setState(v);},
    getValue:function(c) {return c.componentAt(0).state;},
    setEditable:function(c,e) {
        c.componentAt(0).setAvailable(e); 
    }
};
var SRadioCellRenderer={
    render:function(c,p) {
        c.add(sinput('radio',p));
        c.componentAt(0).setValue(p.value);
        c.componentAt(0).setMainClass('cell-radio');
        c.componentAt(0).node.onchange=function(e){
            if(p.action) p.action(e);
        };
    },
    setValue:function(c,v) {c.componentAt(0).setState(v);},
    getValue:function(c) {return c.componentAt(0).state;},
    setEditable:function(c,e) {
        c.componentAt(0).setAvailable(e); 
    }
}
var SSelectCellRenderer={
    render:function(c,p) {
        p.selected=p.value;
        c.add(sselect(p));
        c.componentAt(0).setMainClass('cell-select');
    },
    setValue:function(c,v) {c.componentAt(0).selectAt(v);},
    getValue:function(c) {return c.componentAt(0).selected.value();},
    setEditable:function(c,e) {
        c.componentAt(0).setAvailable(e); 
    }
}
function STableColumn(t,p){
    p=propOverride(p,{label:'column',mode:'default',items:null,sort:false,search:false});
    this.table=t;
    this.choice=null;
    if(p.mode==='default') this.renderer=SCellRenderer;
    else if(p.mode==='check') this.renderer=SCheckCellRenderer;
    else if(p.mode==='radio') this.renderer=SRadioCellRenderer;
    else if(p.mode==='select') {
        this.renderer=SSelectCellRenderer;
        this.choice=p.items;
    }
    SUIComponent.call(this,'th',p);
};
STableColumn.prototype=Object.create(SUIComponent.prototype,{
    initNode:{
        value:function(p){
            SUIComponent.prototype.initNode.apply(this,[p]);
            this.handler=p.handler;
            this.sort=null;
            this.search=null;
            this.header=sview({layout:sflow(HORIZONTAL)});
            this.title=sview({layout:sflow(HORIZONTAL)});
            this.label=slabel(p.label,{class:['column-label']});
            this.sorter=sview({layout:sflow(VERTICAL),class:['column-sorter']});
            this.sorter.add(slabel(TRI_CHARS[TOP],{class:['asc','unselectable']}))
            .add(slabel(TRI_CHARS[BOTTOM],{class:['desc','unselectable']}));
            this.title.add(sspace()).add(this.label).add(sspace());
            this.header.add(this.title).add(this.sorter);
            this.setSuiID('tablecolumn')
            .setMainClass('stable-column')
            .add(this.header)
            .setSortable(p.sort)
            .setSearchable(p.search);
        }
    },
    setSortable:{value:function(s){this.sort=s;this.sorter.setVisible(s); return this;}},
    setSearchable:{value:function(s){this.search=s; return this;}},
    setRenderer:{value:function(r){this.renderer=r; return this;}}
});
STableColumn.prototype.constructor=STableColumn;
function STableRow(t,p){
    p=propOverride(p,{label:'row',values:[]})
    this.table=t;
    this.cells=[];
    SUIComponent.call(this,'tr',p);
};
STableRow.prototype=Object.create(SUIComponent.prototype,{
    initNode:{
        value:function(p){
            SUIComponent.prototype.initNode.apply(this,[p]);
            this.setSuiID('tablerow')
            .addClass('stable-row')
            .addCell(stablecell(this.table,this,this.table.columnAt(0),{value:'',class:['row-header']}))
            .setCells(p.values);
            this.cells[0].setVisible(false);
        }
    },
    addCell:{value:function(c){
        if(c.suiid) this.cells.push(c);
        else this.cells.push(stablecell(this.table,this,this.table.columnAt(this.cells.length),{value:c}));
        this.add(this.cells[this.cells.length-1]);
        return this;
    }},
    setCells:{value:function(v){if(v.length){for(var i=0;i<v.length;i++){this.addCell(v[i]);}}}},
    cellAt:{value:function(i){return this.cells[i];}}
});
STableRow.prototype.constructor=STableRow;
function STableCell(t,r,c,p){
    p=propOverride(p,{value:null});
    this.table=t;
    this.row=r;
    this.column=c;
    SUIComponent.call(this,'td',p);
};
STableCell.prototype=Object.create(SUIComponent.prototype,{
    initNode:{
        value:function(p){
            SUIComponent.prototype.initNode.apply(this,[p]);
            this.setSuiID('tablecell')
            .setMainClass('stable-cell');
            if(this.column.renderer===SSelectCellRenderer) {
                p.items=this.column.choice;
            }
            this.column.renderer.render(this, p);
        }
    },
    setValue:{value:function(v){this.column.renderer.setValue(this,v);}},
    value:{value:function(){this.column.renderer.getValue(this);}}
});
STableCell.prototype.constructor=STableCell;
function STable(p){
    p=propOverride(p,{columns:null,rows:null,data:null,load:null,editable:false,});
    this.header=suic('thead',{class:['table-header']});
    this.body=suic('tbody',{class:['table-body']});
    this.footer=suic('tfoot',{class:['table-footer']})
    this.colview=suic('tr',{class:['column-header']});
    this.header.add(this.colview);
    this.editable=p.editable;
    this.columns=[stablecolumn({label:''})];
    this.rows=[];
    SUIComponent.call(this,'table',p);
};
STable.prototype=Object.create(SUIComponent.prototype,{
    initNode:{
        value:function(p){
            SUIComponent.prototype.initNode.apply(this,[p]);
            const Cls=this;
            if(p.columns&&p.columns.length) {
                for(var c=0;c<p.columns.length;c++) { this.addColumn(p.columns[c]); }
                if(p.rows&&p.rows.length) {
                    for(var r=0;r<p.rows.length;r++) { this.addRow(p.rows[r]); }
                }
                else if(p.data&&p.data.length) {
                    for(var r=0;r<p.data.length;r++) { this.addRow({values:p.data[r]}); }
                }
            }
            else {
                var r=SGET(p.load,'json');
                r.then(function(d){Cls.setData(d);},function(e){console.error(e);});
            }
            this.columns[0].setVisible(false);
            this.setSuiID('table')
            .setMainClass('stable')
            .add(this.header)
            .add(this.body)
            .add(this.footer);
            
        }
    },
    columnAt:{value:function(i){return this.columns[i];}},
    columnCount:{value:function(){return this.columns.length;}},
    addColumn:{value:function(c){
        if(c.suiid) this.columns.push(c);
        else this.columns.push(stablecolumn(this,c));
        this.colview.add(this.columns[this.columns.length-1]);
        return this;
    }},
    setColumn:{value:function(i,c){
        const j=this.colview.components.indexOf(this.columns[i]);
        this.colview.remove(this.columns[i]);
        this.columns[i]=c;
        this.colview.insert(j,this.columns[i]);
        return this;
    }},
    setColumns:{value:function(c){
        this.clearColumns();
        for(var i in c){this.addColumn(i);}
        return this;
    }},
    colunmIndex:{value:function(c){return this.columns.indexOf(c);}},
    clearColumns:{value:function(){
        this.colview.clear();
        this.clolumns=[];
        return this;
    }},
    rowAt:{value:function(i){return this.rows[i];}},
    rowCount:{value:function(){return this.rows.length;}},
    addRow:{value:function(r){
        if(r.suiid) this.rows.push(r);
        else this.rows.push(stablerow(this,r));
        this.body.add(this.rows[this.rows.length-1]);
        return this;
    }},
    setRow:{value:function(i,r){
        const j=this.body.components.indexOf(this.rows[i]);
        this.body.remove(this.rows[i]);
        this.rows[i]=r;
        this.body.insert(j,this.row[i]);
        return this;
    }},
    setRows:{value:function(r){
        this.clearRows();
        for(var i in r){this.addRow(i);}
        return this;
    }},
    rowIndex:{value:function(r){return this.rows.indexOf(r);}},
    clearRows:{value:function(){
        this.body.clear();
        this.rows=[];
        return this;
    }},
    setData:{value:function(d){this.setColumns(d.columns);this.setRows(d.rows);}},
    cellAt:{ value:function(i,j){return this.rows[i].cellAt(j);}}
});
STable.prototype.constructor=STable;


function STableView(p){
   if(p == null) p={};
   this.table=null;
   this.paging;
   this.num;
   this.current;

};
STableView.prototype=Object.create(SUIComponent.prototype,{
    initNode:{
        value:function(p){
            SUIComponent.prototype.initNode.apply(this,[ p ]);
        }
    },
    currentPage:{value:function(){return this.current;}},
    pageCount:{value:function(){ return null;}},
});
STableView.prototype.constructor=STableView;


function STreeNode(p){
    p=propOverride(p,{layout:sflow(VERTICAL),style:NORMAL_TREE,label:'item',leaf:true,state:false,icons:null,children:[]});
    this.style=p.style;
    this.leaf=p.leaf;
    SUIComponent.call(this,'div',p);
};
STreeNode.prototype=Object.create(SUIComponent.prototype,{
    initNode:{
        value:function(p){
            SUIComponent.prototype.initNode.apply(this,[p]);
            const Cls=this;
            this.state=false;
            this.icons=null;
            this.action=p.action;
            this.label=slabel(p.label,{class:['tnode-label','unresponsible','unselectable']});
            this.toggle=sview({class:['tnode-icon','unresponsible','unselectable']});
            if(this.style===NORMAL_TREE){
                this.setMainClass('stree-node');
                if(p.icons) this.icons=[parseIcon(p.icons[0]),parseIcon(p.icons[1]),parseIcon(p.icons[2])];
                else this.icons=[parseIcon(''),parseIcon(TRI_CHARS[RIGHT]),parseIcon(TRI_CHARS[BOTTOM])];
            }
            else if(this.style===FILE_TREE){
                this.setMainClass('sfile-node');
                if(p.icons) this.icons=[parseIcon(p.icons[0]),parseIcon(p.icons[1]),parseIcon(p.icons[2])];
                else this.icons=[parseIcon('icon:insert_drive_file'),parseIcon('icon:folder'),parseIcon('icon:folder_open')];     
            }
            if(this.leaf) this.toggle.add(this.icons[0]);
            this.title=sview({layout:sflow(HORIZONTAL),class:['tree-header']});
            this.title.add(this.toggle).add(this.label);
            this.title.node.onclick=function(e){
               if(!Cls.leaf) Cls.setState(!Cls.state);
               if(Cls.action) Cls.action(e);
            }
            this.children=sview({layout:sflow(VERTICAL),class:['tree-children']});
            if(!this.leaf&&p.children.length){
                for (var c in p.children){
                    if(c.suiid) this.children.add(streenode(c));
                    else this.children.add(c);
                };
            }
            this.setSuiID('treenode')
            .add(this.title)
            .add(this.children)
            .setState(p.state);
        }
    },
    addNode:{value:function(n){
        if(this.leaf) this.leaf=false;
        this.open();
        this.children.add(n);
        return this;
    }},
    open:{value:function(){this.setState(true);}},
    close:{value:function(){this.setState(false);}},
    setState:{value:function(s){
        if(this.leaf) return;
        this.state=s;
        this.toggle.clear();
        this.toggle.add(this.icons[s?2:1]);
        this.children.setVisible(s);
        return this;
    }}
});
STreeNode.prototype.constructor=STreeNode;
function STreeView(p){
    p=propOverride(p,{root:null,showRoot:false});
    SUIComponent.call(this,'div',p);
};
STreeView.prototype=Object.create(SUIComponent.prototype,{
    initNode:{
        value:function(p){
            SUIComponent.prototype.initNode.apply(this,[p]);
            this.showRoot=p.showRoot;
            this.root=null;
            if(p.root){
                if(p.root.suiid) this.root=p.root;
                else this.root=streenode(p.root);
            }
            else this.root=streenode({label:'root'});
            this.setSuiID('treeview')
            .setMainClass('stree-view')
            .update();
        }
    },
    update:{value:function(){
        this.clear();
        if(this.root) {
            if(this.showRoot) this.add(this.root);
            else this.add(this.root.children);
        }
        else return;
    }},
    setRoot:{value:function(root){ 
        this.root=root;
        this.update();
        return this;
    }}
});
STreeView.prototype.constructor=STreeView;


function SMenuItem(p) {
    p=propOverride(p,{style:S_LABEL|S_ICON|S_MODIFIER,state:false,label:'',icon:null,direction:HORIZONTAL,expand:NONE,shortcut:null,submenu:false,items:null,action:null});
    this.style=p.style;
    this.active=false;
    p.layout=sflow(p.direction);
    SUIComponent.call(this,'div',p);
};
SMenuItem.prototype=Object.create(SUIComponent.prototype, {
    initNode: {
        value: function(p) {
            SUIComponent.prototype.initNode.apply(this,[p]);
            var Cls=this;
            this.state=null;
            this.expand=p.expand;

            this.focused=false;
            this.toggle=slabel('',{class:['menu-toggle','unselectable','unresponsible']});
            this.icon=null;
            if(p.icon) this.icon=parseIcon(p.icon).addClass('menu-icon');
            else this.icon=slabel('',{class:['menu-icon','unselectable','unresponsible']});
            this.label=slabel(p.label,{class: ['menu-label','unselectable','unresponsible']});
            if (this.hasClass('vertical')) this.label.addClass('vtext');
            this.submenu=null;
            this.modify=null;
            if (p.submenu) {
                this.submenu=p.submenu;
                this.submenu.trigger=this;
                this.modify=slabel(TRI_CHARS[p.expand],{class: ['menu-expand','unselectable','unresponsible']});
            }
            else if ((this.style&S_MODIFIER)&&p.shortcut) {
                var sc=new SShortCut(p.shortcut,p.action);
                SC_MANAGER.register(sc);
                this.modify=slabel(sc.toString(),{class:['menu-shortcut','unselectable','unresponsible']});
            } 
            else this.modify=slabel('');
            this.action=p.action;
            if (!(p.style&S_TOGGLE)) this.toggle.setVisible(false);
            if (!(p.style&S_ICON)) this.icon.setVisible(false);
            if (!(p.style&S_LABEL)) this.label.setVisible(false);
            if (!(p.style&S_MODIFIER)) this.modify.setVisible(false);
            this.setSuiID('menuitem')
            .addClass('smenu-item')
            .add(this.toggle)
            .add(this.icon)
            .add(this.label)
            .add(sspace())
            .add(this.modify)
            .setState(p.state);

            this.node.onclick=function(e) {
                e.stopPropagation();
                Cls.active=!Cls.active;
                if(Cls.submenu) {
                    if(Cls.active) {
                        Cls.submenu.showAt(Cls,Cls.expand);
                        CURRENT_MENU=Cls.submenu;
                        if(Cls.parent.suiid==='menubar') Cls.parent.setActive(true,Cls);
                    }
                    else Cls.submenu.hide();
                }
                if(Cls.parent.suiid==='menu') {
                    if(Cls.parent.style&S_SELECT) {
                        Cls.parent.selectItem(Cls);
                        Cls.parent.hide();
                        Cls.active=false;
                    }
                }
                if(Cls.action) Cls.action(e);
            }
            this.node.onmouseover=function(e) {
                
                
            }
            this.node.onmouseout=function(e) {
                
                
            }
        }
    },
    setActive: {
        value: function(a) {
            this.active=a;
            if (this.active) {
                this.addClass('active');
                this.showMenu();
            }
            else {
                this.removeClass('active');
                this.hideMenu();
            }
            return this;
        }
    },
    setSelected: {value:function(s){
        this.state=s;
        if (this.style&S_TOGGLE) {
            if (this.state) this.toggle.setText(CHECK_CHAR);
            else this.toggle.setText('');
        }
    }},
    setState: {value:function(s){
        this.state=s;
        if (this.style&S_TOGGLE) {
            if (this.state) {
                this.toggle.setText(CHECK_CHAR);
                if (this.parent&&this.parent.suiid==='menu'&&this.parent.style&S_SELECT) {
                    this.parent.selectItem(this);
                }
            }
            else this.toggle.setText('');
        }
        return this;
    }},
    setLocale: {value:function(l) {
        SUIComponent.prototype.setLocale.apply(this,[l]);
        if(this.submenu) this.submenu.setLocale(l);
    }},
    showMenu: { value: function() {
        if (this.submenu) this.submenu.show(this); }
    },
    hideMenu: { value: function() { 
        if (this.submenu) this.submenu.hide(); }
    },
    setAction: { value: function(a) {
        this.action=a;return this;
    }}
});
SMenuItem.prototype.constructor=SMenuItem;
function SMenu(p) {
    p=propOverride(p,{direction:VERTICAL,mode:null,items:[]});
    this.mode=p.mode;
    p.layout=sflow(p.direction);
    SUIComponent.call(this,'div',p);
}
SMenu.prototype=Object.create(SUIComponent.prototype, {
    initNode: {
        value: function(p) {
            SUIComponent.prototype.initNode.apply(this,[p]);
            var Cls=this;

            this.trigger=null;

            this.active=false;

            this.selected=null;


            this.setSuiID('menu')
            .setMainClass('smenu')
            .setItems(p.items);
        }
    },
    showAt: { 
        value: function(u,l) {
            document.body.appendChild(this.node);
            if(l===BOTTOM||l===TOP) {
                if (document.body.clientWidth<u.X()+this.width()) this.setX(u.X()+u.width()-this.width());
                else this.setX(u.X());
                if (l===BOTTOM) this.setY(u.Y()+u.height());
                else this.setY(u.Y()-this.height());
            }
            else {
                if (document.body.clientHeight<u.Y()+this.height()) this.setY(document.body.clientHeight-this.height());
                else this.setY(u.Y());
                if (l===RIGHT) this.setX(u.X()+u.width());
                else this.setX(u.X()-this.width());
            }
            CURRENT_MENU = this;
            this.active = true;
    }},
    hide: {
        value: function() {
            document.body.removeChild(this.node);
            CURRENT_MENU=null;
            this.active = false;
        }},
    close: {
        value: function() {
            if(this.trigger&&this.trigger.active) this.trigger.setActive(false);
            this.hide();
        }
    },
    selectItem: {
        value: function(i) {
            if (this.selected&&this.mode&S_SELECT) this.selected.setSelected(false);
            this.selected=i;
            this.selected.setSelected(true);
        }
    },
    selectedIndex: {value:function(){return this.selected?this.components.indexOf(this.selected):null;}},
    setItems: {value: function(i) {if(i.length){for(var v=0;v<i.length;v++){this.addItem(i[v]);}}}},
    addItem: {
        value: function(i) {
            if(i.suiid) {
                this.add(i);
                if((this.style&S_SELECT)&&i.state) this.selected=i;
            }
            else {
                if (i.type==='space') this.add(sspace());
                else if (i.type==='bar') this.add(sbar(this.hasClass('vertical')?HORIZONTAL:VERTICAL));
                else {
                    var i_=smenuitem(i);
                    this.add(i_);
                    if((this.style&S_SELECT)&&i.state) this.selected=i_;
                }
            }
        }
    },
    setModes: {value: function(m) {this.mode|=m;}},
});
SMenu.prototype.constructor=SMenu;

function SMenuBar(p) {
    p=propOverride(p,{direction:HORIZONTAL,items:[]});
    this.direction=p.direction;
    this.active=false;
    this.selected=null;
    p.layout=sflow(this.direction);
    SUIComponent.call(this,'div',p);
}
SMenuBar.prototype=Object.create(SUIComponent.prototype, {
    initNode: {
        value: function(p) {
            SUIComponent.prototype.initNode.apply(this,[p]);
            var Cls=this;
            this.setSuiID('menubar')
            .addClass('smenu-bar')
            .setItems(p.items);
            this.node.onclick=function(e) {
                if(Cls.active) {
                    Cls.removeClass('active');
                    if(Cls.selected) Cls.selected.setActive(false);
                    Cls.selected=null;
                    Cls.active=false;
                }
            };
        }
    },
    setActive: {
        value: function(a,i) {
            this.active=a;
            if (this.active) {
                this.addClass('active');
                this.selected=i;
            }
            else {
                this.removeClass('active');
                if(this.selected) this.selectedItem.setActive(false);
                this.selected=null;
            }
            return this;
        }
    },
    setItems: {
        value: function(i){
            if(i.length) for(var v=0;v<i.length;v++){this.addItem(i[v]);}
        }
    },
    addItem: {
        value: function(i) {
            if (i.suiid) this.add(i);
            else {
                if (i.type==='space') this.add(sspace());
                else if (i.type==='bar') this.add(sbar(this.direction==VERTICAL?HORIZONTAL:VERTICAL));
                else this.add(smenuitem(i));
            }
            return this;
        }
    }
});
SMenuBar.prototype.constructor=SMenuBar;

function SToolItem(p) {
    p=propOverride(p,{style:S_ICON|S_LABEL,mode:S_BUTTON,icon:'',label:'item',state:false,action:null,submenu:null,direction:VERTICAL,expand:BOTTOM});
    this.mode=p.mode;
    this.style=p.style;
    this.direction=p.direction;
    p.layout=sflow(p.direction);
    SUIComponent.call(this,'div',p);
};
SToolItem.prototype=Object.create(SUIComponent.prototype, {
    initNode: {
        value: function(p) {
            SUIComponent.prototype.initNode.apply(this,[p]);
            const Cls=this;
            this.icon=null;
            this.label=null;
            this.state=false;
            this.submenu=p.submenu;
            if (p.icon) this.icon=parseIcon(p.icon);
            else this.icon=slabel('',{class:['unselectable','unresponsible']});
            this.icon.addClass('tool-icon');
            this.label=slabel(p.label,{class: ['tool-label','unselectable','unresponsible']});
            if (!(p.style&S_ICON)) this.icon.setVisible(false);
            if (!(p.style&S_LABEL)) this.label.setVisible(false);
            this.setSuiID('toolitem')
            .setMainClass('stool-item')
            .addClass('align-center')
            .setAction(p.action)
            .add(this.icon)
            .add(this.label);
            if (p.mode&S_TOGGLE) this.setState(p.state);
            if (p.submenu) {
                if (p.mode&S_SELECT) p.submenu.setMode(S_SELECT);
            }
            this.node.onclick=function(e) {
                e.stopPropagation();
                if (Cls.hasClass('unavailable')) return;
                if (p.mode&S_TOGGLE) Cls.setState(!Cls.state);
                else if (Cls.submenu) {
                    if (Cls.submenu.active) Cls.submenu.hide();
                    else Cls.submenu.showAt(Cls,p.expand);
                }
                if (Cls.action) Cls.action(e);
            };
        }
    },
    setState: { value: function(s) {
        this.state=s;
        if (this.state) this.addClass('active');
        else this.removeClass('active');
        return this;
    }},
    setAvailable: { value: function(a) {
        if(a) this.removeClass('unavailable');
        else this.addClass('unavailable');
        return this;
    }},
    setAction: { value: function(a) { this.action=a; return this; }},
    setLocale: {value:function(l){
        SUIComponent.prototype.setLocale.apply(this,[l]);
        if (this.submenu) this.submenu.setLocale(l);
    }}
});
SToolItem.prototype.constructor=SToolItem;
function SToolBar(p) {
    p=propOverride(p,{autohide:false,direction:HORIZONTAL,items:[]});
    p.layout=sflow(p.direction);
    this.direction=p.direction;
    SUIComponent.call(this,'div',p);
};
SToolBar.prototype=Object.create(SUIComponent.prototype, {
    initNode: {
        value: function(p) {
            SUIComponent.prototype.initNode.apply(this,[p]);
            const Cls=this;
            this.autohide=p.autohide;
            this.setSuiID('toolbar')
            .setMainClass('stool-bar');
            if (this.autohide) {
                this.addClass('hide');
                this.node.onmouseover=function(e){Cls.show();}
                this.node.onmouseleave=function(e){Cls.hide();}
            }
            if (p.items.length) {
                for (var i=0;i<p.items.length;i++) {
                    this.addItem(p.items[i]);
                }
            }
        }
    },
    addItem: { value: function(i) { 
        if (i.suiid) this.add(i);
        else {
            if(i.type==='space') this.add(sspace());
            else if(p.type==='bar') this.add(sbar(this.direction==HORIZONTAL?VERTICAL:HORIZONTAL));
            else this.add(stbitem(i)); 
        }
        return this;
    }},
    show: { value: function() { 
        this.removeClass('hide');
    }},
    hide: { value: function() { 
        this.addClass('hide');
    }}
});
SToolBar.prototype.constructor=SToolBar;



function SForm(p){p=propOverride(p,{key:'api',method:'post'});SUIComponent.call(this,'form',p);};
SForm.prototype=Object.create(SUIComponent.prototype,{
    initNode:{value:function(p) {
        SUIComponent.prototype.initNode.apply(this,[p]);
        this.setSuiID('form').setMainClass('sform').setAttribute({action:p.key}).setAttribute('method',p.method);
    }},
    submit:{value:function(){this.node.submit();}},
});
SForm.prototype.constructor=SForm;
function SInput(t,p){p=propOverride(p,{value:null});SUIComponent.call(this,'input',p);this.node.type=t;};
SInput.prototype=Object.create(SUIComponent.prototype,{
    initNode:{value:function(p){
        SUIComponent.prototype.initNode.apply(this,[p]);
        this.setSuiID('input').setMainClass('sinput').setValue(p.value);
    }},
    value:{value:function(){return this.node.value;}},
    setValue:{value:function(v){if(v) this.node.value=v; return this;}}
});
SInput.prototype.constructor=SInput;
function STextField(p){
    p=propOverride(p,{type:'text',label:'',value:'',placeholder:'',selectable:true,editable:true,required:false});
    SUIComponent.call(this,'label',p);
};
STextField.prototype=Object.create(SUIComponent.prototype, {
    initNode:{value:function(p){
        SUIComponent.prototype.initNode.apply(this,[p]);
        this.label=slabel(p.label,{class:['field-label']});
        this.input=sinput(p.type,p);
        if (p.type==='number'){this.setSuiID('numericfield').setMainClass('snumber-field');}
        else if (p.type==='password'){this.setSuiID('passfield').setMainClass('spass-field');}
        else if (p.type==='text'){this.setSuiID('textfield').setMainClass('stext-field');}
        this.add(this.label)
        .add(this.input)
        .setValue(p.value)
        .setPlaceholder(p.placeholder)
        .setSelectable(p.selectable)
        .setEditable(p.editable)
        .setRequired(p.required);
    }},
    placeholder:{value:function() {return this.input.node.placeholder;}},
    setValue:{value:function(v){if(v) this.input.node.value=v; return this;}},
    setPlaceholder:{value:function(t) {if(t) this.input.node.placeholder=t; return this;}},
    isSelectable:{value:function() {return !this.hasClass('unselectable');}},
    setSelectable:{value:function(s){if(s) this.removeClass('unselectable'); else{this.addClass('unselectable');} return this;}},
    isEditable:{value:function() {return !this.input.node.readOnly;}},
    setEditable:{value:function(e) {this.input.node.readOnly=!e; return this;}},
    isReqired:{value:function() {return this.input.node.required;}},
    setRequired:{value:function(r) {this.input.node.required=r; return this;}}
});
STextField.prototype.constructor=STextField;
function STextArea(p) {
    p=propOverride(p,{label:'',value:'',placeholder:'',selectable:true,editable:true,required:false});
    SUIComponent.call(this,'label',p);
};
STextArea.prototype=Object.create(SUIComponent.prototype, {
    initNode:{
        value:function(p) {
            SUIComponent.prototype.initNode.apply(this,[{}]);
            this.label=slabel(p.label, {class:['valign-top'] });
            this.area=sinput('textarea',p);
            this.setSuiID('textarea')
            .setMainClass('stext-area')
            .add(this.label)
            .add(this.area)
            .setValue(p.text)
            .setPlaceholder(p.placeholder)
            .setSelectable(p.selectable==undefined?true:p.selectable)
            .setEditable(p.editable==undefined?true:p.editable)
            .setRequired(p.required==undefined?false:p.required);
        }
    },
    value:{value:function() {return this.area.node.innerText;}},
    setValue:{value:function(txt) {if (txt!=undefined) this.area.node.innerText=txt; return this;}},
    placeholder:{value:function() {return this.node.placeholder;}},
    setPlaceholder:{value:function(txt) {if (txt!=undefined) this.node.placeholder=txt; return this;}},
    isSelectable:{value:function() {return !this.hasClass('unselectable');}},
    setSelectable:{value:function(sel) {
        if (sel) this.removeClass('unselectable');
        else this.addClass('unselectable');
        return this;
    }},
    isEditable:{value:function() {return !this.node.readOnly;}},
    setEditable:{value:function(edit) {this.node.readOnly=!edit; return this;}},
    isReqired:{value:function() {return this.node.required;}},
    setRequired:{value:function(req) {this.node.required=req; return this;}}
});
STextArea.prototype.constructor=STextArea;

function STextEditor(p) {

    SUIComponent.call(this,'pre',p);
};
STextEditor.prototype=Object.create(SUIComponent.prototype, {
    initNode:{
        value:function(p) {
            SUIComponent.prototype.initNode.apply(this, [ p ]);
            this.className='textbox';
            this.setMainClass('stext-box');
            this.setEditable(p.editable?true:false);
            this.setContent(p.content==undefined?'':p.content);
        }
    },
    isEditable:{value:function() {return this.node.contentEditable;}},
    setEditable:{value:function(e) {this.node.contentEditable=e;}},
    setContent:{value:function(content) {this.node.innerHTML=content;}}
});
STextEditor.prototype.constructor=STextEditor;
function SButton(p){
    p=propOverride(p,{label:'',icon:null,state:false,available:true,action:null,style:DEFAULT});
    if(p.style) SUIComponent.call(this,'div',p);
    else SUIComponent.call(this,'button',p);
};
SButton.prototype=Object.create(SUIComponent.prototype,{
    initNode:{value:function(p){
        SUIComponent.prototype.initNode.apply(this,[p]);
        const Cls=this;
        this.setSuiID('button')
        .setState(p.state)
        .setAction(p.action);
        if(p.style===S_ICON){
            this.image=parseIcon(p.icon).addClass('button-image');
            this.setMainClass('simg-button').add(this.image);
            this.node.onclick=function(e){if(Cls.action){Cls.action(e);}};
        }
        else if(p.style===S_TOGGLE) {
            this.setMainClass('stoggle-button');
            this.node.onclick=function(e){Cls.setState(!Cls.state); if(Cls.action){Cls.action(e);}};
        }
        else if (p.style===S_PANEL) {
            this.setMainClass('spane-button');
            this.node.onclick=function(e){if(Cls.action){Cls.action(e);}};
        }
        else{
            this.label=slabel(p.label,{class:['unselectable','unresponsible']});
            this.setMainClass('sbutton').add(this.label);
            this.node.onclick=function(e){if(Cls.action){Cls.action(e);}};
        }
    }},
    setState:{value:function(s){this.state=s;
        if (this.state) this.addClass('active');
        else this.removeClass('active');
        return this;
    }},
    setAction:{value:function(a){this.action=a; return this;}}
});
SButton.prototype.constructor=SButton;
function SRadioButton(p) {p=propOverride(p,{label:'',state:false,available:true,action:null});SUIComponent.call(this,'label',p);};
SRadioButton.prototype=Object.create(SUIComponent.prototype, {
    initNode:{
        value:function(p) {
            SUIComponent.prototype.initNode.apply(this,[{}]);
            this.label=slabel(p.label,{class:['sradio-label']});
            this.input=sinput('radio',p);
            this.setSuiID('radio')
            .setMainClass('sradio')
            .add(this.input)
            .add(this.label)
            .setState(p.state)
            .setAvailable(p.available)
            .setAction(p.action);
        }
    },
    state:{value:function(){return this.input.node.checked;}},
    setState:{value:function(s){this.input.node.checked=s;return this;}},
    isAvailable:{value:function() {return !this.input.isAvailable();}},
    setAvailable:{value:function(a) {this.input.setAvailable(a);return this;}},
    setAction:{value:function(a) {if(a) this.input.node.onchange=a;return this;}}
});
SRadioButton.prototype.constructor=SRadioButton;
function SCheckBox(p) {p=propOverride(p,{label:'',state:false,available:true,action:null});SUIComponent.call(this,'label',p);};
SCheckBox.prototype=Object.create(SUIComponent.prototype, {
    initNode:{
        value:function(p) {
            SUIComponent.prototype.initNode.apply(this, [{}]);
            this.label=slabel(p.label,{class:['scheck-label']});
            this.input=sinput('checkbox',p);
            this.setSuiID('checkbox')
            .setMainClass('scheckbox')
            .add(this.input)
            .add(this.label)
            .setState(p.state)
            .setAvailable(p.available)
            .setAction(p.action);
        }
    },
    state:{value:function() {return this.input.node.checked;}},
    setState:{value:function(s) {this.input.node.checked=s;return this;}},
    isAvailable:{value:function() {return !this.input.isAvailable();}},
    setAvailable:{value:function(a) {this.input.setAvailable(a);return this;}},
    setAction:{value:function(a) {if(a) this.input.node.onchange=a;return this;}}
});
SCheckBox.prototype.constructor=SCheckBox;
function SSelectItem(p) {
    p=propOverride(p,{group:false,value:'item',label:'item'});
    if (p.group) SUIComponent.call(this,'optgroup',p);
    else SUIComponent.call(this,'option',p);
};
SSelectItem.prototype=Object.create(SUIComponent.prototype, {
    initNode:{
        value:function(p) {
            SUIComponent.prototype.initNode.apply(this,[p]);
            this.label=slabel(p.label);
            if (this.node.tagName==='OPTION') {
                this.setSuiID('selectitem')
                .setMainClass('sselect-item')
                .add(this.label)
                .setValue(p.value);
            }
            else {
                this.setSuiID('selectgroup')
                .setMainClass('sselect-group')
                .add(this.label);
            }
        }
    },
    setLabel:{value:function(l) {this.label.setText(l);}},
    setValue:{value:function(v) {this.node.value=v; return this;}}
});
SSelectItem.prototype.constructor=SSelectItem;
function SSelector(p){p=propOverride(p,{multi:false,selected:1,items:[],action:null});SUIComponent.call(this,'select',p);};
SSelector.prototype=Object.create(SUIComponent.prototype, {
    initNode:{
        value:function(p) {
            SUIComponent.prototype.initNode.apply(this,[p]);
            this.setSuiID('selector')
            .setMainClass(['sselector'])
            .setMultiSelectable(p.multi)
            .setOptions(p.items)
            .selectAt(p.selected)
            .setAction(p.action);
        }
    },
    setMultiSelectable:{value:function(m){this.node.multiple=m; return this;}},
    setOptions:{value:function(o){for(var i=0;i<o.length;i++){this.addOption(o[i]);} return this;}},
    select:{value:function(o){o.select(true); return this;}},
    selectAt:{value:function(i){if (i<=this.components.length){this.node.selectedIndex=i-1;} return this;}},
    addOption:{value:function(o) {
        if (o.suiid==='selectitem'||o.suiid==='selectgroup') this.add(o);
        else this.add(sselitem(o));
        return this;
    }},
    setAction:{value:function(a) {if(a) this.node.onchange=function(e){a(e);}; return this;}},
    selectedIndex:{value:function() {return this.node.selectedIndex;}},
    selectedItem:{value:function() {return this.node.selectedOptions[0];}},
    selectedItems:{value:function() {return this.node.selectedOptions;}}
});
SSelector.prototype.constructor=SSelector;
function SSlider(p) {p=propOverride(p,{label:'',min:0,max:100,value:0,action:null});SUIComponent.call(this,'label',p);};
SSlider.prototype=Object.create(SUIComponent.prototype, {
    initNode:{
        value:function(p) {
            SUIComponent.prototype.initNode.apply(this,[{}]);
            this.label=slabel(p.label,{class:['slider-label']});
            this.input=sinput('range',p);
            this.setSuiID('slider')
            .setMainClass('sslider')
            .add(this.input)
            .add(this.label)
            .setMin(p.min)
            .setMax(p.max)
            .setValue(p.value)
            .setAction(p.action);
        }
    },
    minValue:{value:function() {return this.input.node.min;}},
    setMin:{value:function(m) {this.input.node.min=m; return this;}},
    maxValue:{value:function() {return this.input.node.max;}},
    setMax:{value:function(m) {this.input.node.max=m; return this;}},
    current:{value:function() {return this.input.node.value;}},
    setValue:{value:function(v) {
        this.input.node.value=v; 
        this.input.node.caption=v;
        return this;
    }},
    setAction:{value:function(a) {if(a) this.input.node.onchange=function(e){a(e);}; return this;}}
});
SSlider.prototype.constructor=SSlider;
function SDatePicker(p) {
    if (p == null) p={};
    p.sclass='datepicker';
    if (p.class == undefined) p.class=['sdatepicker'];
    else p.class.push('sdatepicker');
    SUIComponent.call(this, 'input', p);
};
SDatePicker.prototype=Object.create(SUIComponent.prototype, {
    initNode:{
        value:function(p) {
            SUIComponent.prototype.initNode.apply(this, [ p ]);
            this.node.type='date';
            this.setFormat(p.format==undefined?'yyyy/mm/dd':p.format);
            this.setValue(p.value==undefined?(new Date()).toDateString():p.value);
        }
    },
    setFormat:{value:function(form) {this.node.format=form;}},
    setValue:{value:function(val) {this.node.value=val;}}
});
SDatePicker.prototype.constructor=SDatePicker;
function SColorPicker(p) {
    if (p == null) p={};
    SUIComponent.call(this, 'input', p);
};
SColorPicker.prototype=Object.create(SUIComponent.prototype, {
    initNode:{
        value:function(p) {
            SUIComponent.prototype.initNode.apply(this, [ p ]);
            this.className='colorpicker';
            this.addClass('scolor-picker');
            this.node.type='color';
        }
    }
});
SColorPicker.prototype.constructor=SColorPicker;
function SFileLoader(p) {
    p=propOverride(p,{id:'sfl',label:'file-select',filter:null,action:null});
    this.accessible=true;
    this.files=null;
    SUIComponent.call(this,'div',p);
};
SFileLoader.prototype=Object.create(SUIComponent.prototype, {
    initNode:{
        value:function(p) {
            SUIComponent.prototype.initNode.apply(this,[{}]);
            const Cls=this;
            this.input=sinput('file',{id:p.id,visible:false});
            this.input.node.onchange=function(e){
                Cls.files=this.files;
                if(p.action) p.action(e);
            };
            if(p.filter) this.input.node.accept=p.filter;
            this.label=suic('label');
            this.label.setMainClass('fl-label');
            this.label.add(slabel(p.label,{class:['unselectable','unresponsible']}));
            this.label.node.htmlFor=p.id;
            this.setSuiID('fileloader')
            .setMainClass('sfileloader')
            .add(this.input)
            .add(this.label);
        }
    }
});
SFileLoader.prototype.constructor=SFileLoader;
function SLabel(t,p) {p=propOverride(p,{});SUIComponent.call(this,'span',p);this.setText(t);};
SLabel.prototype=Object.create(SUIComponent.prototype, {
    initNode:{value:function(p) {
        SUIComponent.prototype.initNode.apply(this,[p]);
        this.txt='';
        this.setSuiID('label').setMainClass('slabel');
    }},
    text:{value:function(){return this.node.innerText;}},
    setText:{value:function(t) {
        this.txt=t;
        if (this.isLocalized()&&localizable(this.locale,t)) this.node.innerHTML=LOCAL_LABEL[t][this.locale];
        else this.node.innerHTML=t; 
        return this;
    }},
    setLocale:{value:function(l){SUIComponent.prototype.setLocale.apply(this,[l]); this.setText(this.txt); return this;}}
});
SLabel.prototype.constructor=SLabel;
function SLinkLabel(t,u,p) {p=propOverride(p,{newtab:false});SUIComponent.call(this,'a',p);this.setText(t).setURL(u);};
SLinkLabel.prototype=Object.create(SUIComponent.prototype, {
    initNode:{
        value:function(p) {
            SUIComponent.prototype.initNode.apply(this,[p]);
            this.txt='';
            this.setSuiID('link').setMainClass('slink');
            if (p.newtab) this.setAttribute({target:'_blank',rel:'noopener noreferrer'});
        }
    },
    text:{value:function() {return this.node.innerText;}},
    setText:{value:function(t) {
        this.txt=t;
        if (this.isLocalized() && localizable(this.locale,t)) this.node.innerText=LOCAL_LABEL[t][this.locale];
        else this.node.innerText=t;
        return this;
    }},
    setURL:{value:function(url){this.node.href=url; return this;}},
    setLocale:{
        value:function(loc) {
            SUIComponent.prototype.setLocale.apply(this,[loc]);
            this.setText(this.txt); return this;
        }
    }
});
SLinkLabel.prototype.constructor=SLinkLabel;
function SIcon(i,p) {p=propOverride(p,{});SUIComponent.call(this,'i',p);this.node.innerText=i;};
SIcon.prototype=Object.create(SUIComponent.prototype, {
    initNode:{value:function(p) {
        SUIComponent.prototype.initNode.apply(this,[p]);
        this.setSuiID('icon').setMainClass('sicon');
    }}
});
SIcon.prototype.constructor=SIcon;
function SProgress(p) {
    p=propOverride(p,{min:0,max:100,value:0,direction:HORIZONTAL,repaint:null});
    this.direction=p.direction;    
    SUIComponent.call(this,'div',p);
};
SProgress.prototype=Object.create(SUIComponent.prototype, {
    initNode: {value:function(p){
        SUIComponent.prototype.initNode.apply(this,[p]);
        this.setSuiID('progress')
        .setMainClass('sprogress');
        this.min=p.min;
        this.max=p.max;
        this.value=p.value;
        this.spacer=sspace();
        this.indicator=sview({width:'100%',height:'100%',class:['prog-indicator']});
        this.repaint=p.repaint;
        switch(this.direction) {
            case HORIZONTAL:
            {
                this.setLayout(sflow(HORIZONTAL))
                .addClass('horizontal')
                .add(this.indicator)
                .add(this.spacer);
                break;
            }
            case VERTICAL:
            {
                this.setLayout(sflow(VERTICAL))
                .addClass('vertical')
                .add(this.spacer)
                .add(this.indicator);
                break;
            }
        }        
    }},
    getValue:{value:function() {return current;}},
    setValue:{value: function(v) {
        if (v<=this.max&&this.min<=v) {
            this.value=v;
            this.update();
        }
    }},
    update:{value:function() {
        switch(this.direction) {
            case HORIZONTAL:
                {
                    if (this.repaint) this.repaint(this.value);
                    else this.spacer.setWidth((1.0-(this.value-this.min)/(this.max-this.min))*100+'%');
                }
                break;
            case VERTICAL:
                {
                    if (this.repaint) this.repaint(this.value);
                    else this.spacer.setHeight((1.0-(this.value-this.min)/(this.max-this.min))*100+'%');
                }
                break;
            default:
                {
                    if (this.repaint) this.repaint(this.value);
                }
                break;
        }
    }}
});
SProgress.prototype.constructor=SProgress;
function STimer(d,f,r){
    this.thread=null;
    this.active=false;
    this.rep=r;
    this.duration=d;
    this.action=f;
};
STimer.prototype={
    start:function(){
        var Cls=this;
        if (1 < this.rep) this.thread=setInterval(function(){Cls.action(Cls.thread, Cls.rep);}, Cls.duration);
        else this.thread=setTimeout(function(){Cls.action(Cls.thread);}, Cls.duration);
   },
    stop:function(){
        if (1 < this.rep) clearInterval(this.thread); 
        else clearTimeout(this.thread);
   }
};