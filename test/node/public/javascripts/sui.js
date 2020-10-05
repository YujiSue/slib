MathJax={tex:{inlineMath:[['$','$'],['\\(','\\)']]}};
const PC_DEVICE=0x01;
const MOBILE_DEVICE=0x02;
const WINDOWS_DEVICE=0x10;
const APPLE_DEVICE=0x20;
const LINUX_DEVICE=0x40;
const UNIX_DEVICE=0x80;
var CLIENT_ENV=0;
if(platform){
    const o=platform.os.toString().toLowerCase();
    if(o.indexOf("windows")!==-1) CLIENT_ENV=PC_DEVICE|WINDOWS_DEVICE;
    else if(o.indexOf("mac")!==-1) CLIENT_ENV=PC_DEVICE|APPLE_DEVICE;
    else if(o.indexOf("os x")!==-1) CLIENT_ENV=PC_DEVICE|APPLE_DEVICE;
    else if(o.indexOf("android")!==-1) CLIENT_ENV=MOBILE_DEVICE|LINUX_DEVICE;
    else if(o.indexOf("ios")!==-1) CLIENT_ENV=MOBILE_DEVICE|APPLE_DEVICE;
}
var PARSER=new DOMParser();
if(CLIENT_ENV===MOBILE_DEVICE|APPLE_DEVICE){
    (function(DOMParser){"use strict";
        var p=DOMParser.prototype,nativeParse=p.parseFromString;
        try{if((new DOMParser()).parseFromString("","text/html")){return;}}catch(e){}
        p.parseFromString=function(m,t){
            if(/^\s*text\/html\s*(?:;|$)/i.test(t)){
                var d=document.implementation.createHTMLDocument("");
                if(m.toLowerCase().indexOf('<!doctype')>-1){d.documentElement.innerHTML=m;}
                else{d.body.innerHTML=m;} return d;
            }else{return nativeParse.apply(this,arguments);}
        };
    }(DOMParser));
}
if(!String.prototype.startsWith){String.prototype.startsWith=function(s,p){return this.substr((p||0),s.length)===s;};}
if(!String.prototype.endsWith){String.prototype.endsWith=function(s,p){return this.substr(this.length-s.length-(p||0),s.length)===s;};}
if(!String.prototype.padStart){
    String.prototype.padStart=function padStart(l,s){
        l=l>>0;s=String(typeof s!=='undefined'?padString:' ');
        if(this.length>=l) return String(this);
        else{l=l-this.length;if(l>s.length){s+=s.repeat(l/s.length);}return s.slice(0,l)+String(this);}
    };
}
if(!DOMTokenList.prototype.replace){DOMTokenList.prototype.replace=function(o,n){const v=String(DOMTokenList.value);DOMTokenList.value=v.replace(o,n);};}
if(typeof TextEncoder==="undefined"){
    TextEncoder=function TextEncoder(){};
    TextEncoder.prototype.encode=function encode(s){
        "use strict";
        var l=s.length,r=-1;var a=typeof Uint8Array==="undefined"?new Array(l*1.5):new Uint8Array(l*3);
        for(var p=0,n=0,i=0;i!==l;){
            p=s.charCodeAt(i), i+=1;
            if(p>=0xD800&&p<=0xDBFF){
                if(i===l){a[r+=1]=0xef;a[r+=1]=0xbf;a[r+=1]=0xbd;break;}
                n=s.charCodeAt(i);
                if(n>=0xDC00&&n<=0xDFFF){
                    p=(p-0xD800)*0x400+n-0xDC00+0x10000;i+=1;
                    if(p>0xffff){a[r+=1]=(0x1e<<3)|(p>>>18);a[r+=1]=(0x2<<6)|((p>>>12)&0x3f);a[r+=1]=(0x2<<6)|((p>>>6)&0x3f);a[r+=1]=(0x2<<6)|(p&0x3f);continue;}
                } 
                else{a[r+=1]=0xef;a[r+=1]=0xbf;a[r+=1]=0xbd;continue;}
            }
            if(p<=0x007f){a[r+=1]=(0x0<<7)|p;} 
            else if(p<=0x07ff){a[r+=1]=(0x6<<5)|(p>>>6);a[r+=1]=(0x2<<6)|(p&0x3f);}
            else{a[r+=1]=(0xe<<4)|(p>>>12);a[r+=1]=(0x2<<6)|((p>>>6)&0x3f);a[r+=1]=(0x2<<6)|(p&0x3f);}
        }
        if(typeof Uint8Array!=="undefined") return a.subarray(0,r+1);
        a.length=r+1; return a;
    };
    TextEncoder.prototype.toString=function(){return "[object TextEncoder]"};
    try{Object.defineProperty(TextEncoder.prototype,"encoding",{
            get:function(){if(TextEncoder.prototype.isPrototypeOf(this)){return"utf-8";} else throw TypeError("Illegal invocation");}
        });
    }catch(e){TextEncoder.prototype.encoding="utf-8"; }
    if(typeof Symbol!=="undefined")TextEncoder.prototype[Symbol.toStringTag]="TextEncoder";
};
function codedString(s){return PARSER.parseFromString(s,"text/html").body.innerText;};
const SPACE_CHAR=codedString('&nbsp;');
const AMP_CHAR=codedString('&amp;');
const QUOT_CHAR=codedString('&quot;');
const DEGREE_CHAR=codedString('&deg;');
const DOT_CHAR=codedString('&middot;');
const CIRC_CHAR=codedString('&circ;');
const TILDA_CHAR=codedString('&tilde;');
const LT_CHAR=codedString('&lt;');
const GT_CHAR=codedString('&gt;');
const CHECK_CHAR=codedString('&#10003;');
const POST_CHAR=codedString('&#12306;');
const COPYWRITE_CHAR=codedString('&copy;');
const TRI_CHARS=[codedString('&#9650;'),codedString('&#9660;'),codedString('&#9664;'),codedString('&#9654;')];
const CROSS_CHAR=codedString('&#215;');
const COMMAND_CHAR=codedString('&#8984;');
const CTRL_CHAR=codedString('&#8963;');
const SHIFT_CHAR=codedString('&#8679;');
const OPT_CHAR=codedString('&#8997;');

const SHIFT_MASK=0x01;
const CTRL_MASK=0x02;
const ALT_MASK=0x04;
const CMD_MASK=0x08;
const DEFAULT_CMD_MASK=0x10;

const DEFAULT=0x00;
const ORIGINAL=0x01;
const NOT_FOUND=-1;

const S_ICON=0x0001;
const S_LABEL=0x0002;
const S_BUTTON=0x0004;
const S_SELECT=0x0008;
const S_TOGGLE=0x0010;
const S_PANEL=0x0020;
const S_MODIFIER=0x0040;

//Tree
var NORMAL_TREE=0x01;
var FILE_TREE=0x02;


const VERTICAL=0x01;
const HORIZONTAL=0x02;
const DEPTH=0x04;
const DIVERGENCE=0x11;
const CONVERGENCE=0x12;
const CW=0x21;
const CCW=0x22;
const TOP=0x00;
const BOTTOM=0x01;
const LEFT=0x02;
const RIGHT=0x03;
const NONE=0xFF;

var CURRENT_MENU=null;

var SKEY_STATE={source:null,pushed:false,mask:0,key:''};
var SKEY_EVENTLISTENER={down:[],pressed:[],up:[]};
function SShortCut(s,a){
    this.key=s.key;this.mask=s.mask;
    if(this.mask&DEFAULT_CMD_MASK){
        if(CLIENT_ENV&APPLE_DEVICE) this.mask=this.mask-DEFAULT_CMD_MASK+CMD_MASK;
        else this.mask=this.mask-DEFAULT_CMD_MASK+CTRL_MASK;
    }
    this.action=a;
};
SShortCut.prototype={
    toString: function(){
        var str='';
        if(CLIENT_ENV&APPLE_DEVICE){
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
        str+=this.key.toUpperCase(); return str;
    }
};
function SSCManager(){this.sclist={};};
SSCManager.prototype={
    register: function(s){
        if(this.sclist[s.key]) this.sclist[s.key][s.mask]=s.action;
        else {this.sclist[s.key]=new Array(16);this.sclist[s.key][s.mask]=s.action;}
    },
    fire: function(e){
        if(this.sclist[SKEY_STATE.key]&&this.sclist[SKEY_STATE.key][SKEY_STATE.mask]){this.sclist[SKEY_STATE.key][SKEY_STATE.mask](e);}
    }
};
function addSKeyEventListener(t,a){SKEY_EVENTLISTENER[t].pushed(a);}
var SC_MANAGER=new SSCManager();
document.addEventListener('keydown',function(e){
    if(e.target&&e.target.sui) SKEY_STATE.source=e.target.sui;
    SKEY_STATE.pushed=true;
    if(e.altKey) SKEY_STATE.mask|=ALT_MASK;
    if(e.shiftKey) SKEY_STATE.mask|=SHIFT_MASK;
    if(e.ctrlKey ) SKEY_STATE.mask|=CTRL_MASK;
    if(e.metaKey ) SKEY_STATE.mask|=CMD_MASK;
    SKEY_STATE.key=e.key;
    if(SKEY_EVENTLISTENER['down'].length){
        for(var a=0;a<SKEY_EVENTLISTENER['down'].length;a++){SKEY_EVENTLISTENER['down'][a](e);}
    }
    SC_MANAGER.fire(e);
});
document.addEventListener('keypress', function(e){
    if(SKEY_EVENTLISTENER['pressed'].length){
        for(var a=0;a<SKEY_EVENTLISTENER['pressed'].length;a++){SKEY_EVENTLISTENER['pressed'][a](e);}
    }
});
document.addEventListener('keyup', function(e){
    if(SKEY_EVENTLISTENER['up'].length){
        for(var a=0;a<SKEY_EVENTLISTENER['up'].length;a++){SKEY_EVENTLISTENER['up'][a](e);}
    }
    SKEY_STATE.source=null;
    SKEY_STATE.pushed=false;
    SKEY_STATE.mask=0;
    SKEY_STATE.key='';
});












function inRange(x,y,s){if(s){return x>=s.X()&&x<=(s.X()+s.width())&&y>=s.Y()&&y<=s.Y()+s.height();} return false;};
function menuRoot(sui){

};
function encodeForm(d){var params=[];for(var k in d){params.push(encodeURIComponent(k)+'='+encodeURIComponent(d[k]));} return params.join('&').replace(/%20/g,'+');};
function propOverride(p,o){if(!p){p=o;}else{for(var k in o){if(Object.keys(p).indexOf(k)==-1) p[k]=o[k];}} return p;};
function instanceOfID(i,s){if(!s){s=document;} const e=s.getElementById(i); if(e){return e.sui;} else return null;};
function supload(f){
    var i=document.getElementById('_FILE_LOADER_');
    if(!i){
        i=document.createElement('input');
        i.setAttribute('type','file');
        i.setAttribute('id','_FILE_LOADER_');
        i.onchange=function(e){if(f){f(e);}i.remove();};
        i.style.display='none';
        document.body.appendChild(i);
    }
    i.click();
};
function sdownload(n,b){
    const u=URL.createObjectURL(b);
    const a=document.createElement("a");
    document.body.appendChild(a);
    a.download=n;
    a.href=u;a.click();
    setTimeout(function(){
        URL.revokeObjectURL(u)
        document.body.removeChild(a)
      }, 500);
};
function sget(u,t){
    return new Promise(function(resolve,reject){
        var xhr=new XMLHttpRequest();xhr.open('GET',u);xhr.responseType=t;xhr.send();
        xhr.onreadystatechange=function(){
            if(xhr.readyState===XMLHttpRequest.DONE){
                if(xhr.status===200){resolve(xhr.response);} else{reject();}
            }
        }
    });
};
function spost(u,t,d){
    return new Promise(function(resolve,reject){
        var xhr=new XMLHttpRequest();xhr.open('POST',u);xhr.setRequestHeader('Content-Type',t);xhr.send(d);
        xhr.onreadystatechange=function(){
            if(this.readyState===XMLHttpRequest.DONE){
                if(xhr.status===200){resolve(xhr.response);} else{reject(xhr.status);}
            }
        }
    });
};
function parseValue(v){
    if(typeof v==='number') return v+'px';
    else if(typeof v==='string') return v;
}
function parseImage(s,c){
    if(s.startsWith('icon:')) return sicon(s.substring(5),{class:['unselectable','unresponsible']});
    else if(s.startsWith('image:')) return simgview({src:s.substring(6),class:['unselectable','unresponsible']});
    else return slabel(s,{class:['unselectable','unresponsible']});
};
function useHash(t,f){
    const e=new TextEncoder().encode(t);
    if(crypto.subtle&&crypto.subtle.digest){
        crypto.subtle.digest('SHA-256',e).then(function(d){
            const a=Array.from(new Uint8Array(d));
            var s='';
            for(var i=0;i<a.length;i++){s+=a[i].toString(16).padStart(2,'0');}
            if(f) f(s);
        });
    }
    else if(crypto.webkitSubtle){
        crypto.webkitSubtle.digest('SHA-256',e).then(function(d){
            const a=Array.from(new Uint8Array(d));var s='';
            for(var i=0;i<a.length;i++){s+=a[i].toString(16).padStart(2,'0');}
            if(f) f(s);
        });
    }
};
function SAjax(){};
SAjax.prototype={
    get: function(p){
        p=propOverride(p,{url:'',type:'',next:null,error:null});
        const xhr=new XMLHttpRequest();xhr.open('GET',p.url);xhr.responseType=p.type;xhr.send();
        xhr.onreadystatechange=function(){
            if(xhr.readyState===XMLHttpRequest.DONE){
                if(xhr.status===200&&p.next){p.next(xhr.response);} else if(p.error){p.error(xhr.status);}
            }
        }
    },
    post: function(p){
        p=propOverride(p,{url:'',data:null,next:null,error:null});
        var xhr=new XMLHttpRequest();xhr.open('POST',p.url);xhr.setRequestHeader('Content-Type','application/x-www-form-urlencoded');xhr.send(encodeForm(p.data));
        xhr.onreadystatechange=function(){
            if(this.readyState===XMLHttpRequest.DONE){
                if(xhr.status===200&&p.next){p.next(xhr.response);} else if(p.error){p.error(xhr.status);}
            }
        }
    }
};
function sajax(){return new SAjax();}
function SCookie(p){
    p=propOverride(p,{samesite:'lax',secure:true,path:null,age:null,expires:null});
    if(p.samesite) document.cookie='samesite='+p.samesite;
    if(p.secure) document.cookie='secure';
    if(p.path) document.cookie='path='+p.path;
    if(p.expires) document.cookie='expires='+p.expires;
    else if(p.age) document.cookie='max-age='+p.age;
};
SCookie.prototype={
    set: function(k,v){document.cookie=k+'='+v;},
    at: function(k){
        const c=document.cookie.split('; ');
        for(i=0;i<c.length;i++){if(c[i].startsWith(k+'=')){return c[i].split('=')[1];}}
        return null;
    },
    data:function(){
        var d={};
        const c=document.cookie.split('; ');
        for(i=0;i<c.length;i++){
            if(c[i].indexOf('=')!=-1){const v= c[i].split('=');d[v[0]]=v[1];}
        }
        return d;
    },
    setExpireSec: function(s){document.cookie='max-age='+s;},
    setExpire:function(d){document.cookie='expires='+d},
    clear:function(){document.cookie='max-age=-1';}
};

function SSocket(n){
    this.socket=io();
    this.connectID=n;
    this.response=null;
    const Cls=this;
    this.socket.on(Cls.connectID,function(d){
        if(Cls.response) Cls.response(d);
    });
};
SSocket.prototype={
    send: function(d){this.socket.emit(this.connectID,d);},
    setResponse:function(r){this.response=r;}
};







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
    if(e.target&&e.target.sui) SMOUSE_STATE.source=e.target.sui;
    SMOUSE_STATE.pushed=true;
    SMOUSE_STATE.dragging=false;
    SMOUSE_STATE.oriX=e.clientX;
    SMOUSE_STATE.oriY=e.clientY;
});
document.addEventListener('mousemove',function(e){
    if(SMOUSE_STATE.pushed){
        SMOUSE_STATE.dragging=true;
        SMOUSE_STATE.diffX=e.clientX-SMOUSE_STATE.oriX;
        SMOUSE_STATE.diffY=e.clientY-SMOUSE_STATE.oriY;
        SMOUSE_STATE.oriX=e.clientX;
        SMOUSE_STATE.oriY=e.clientY;
        if(SMOUSE_STATE.source.suiid==='splitter') SMOUSE_STATE.source.update(e);
        //console.log(SMOUSE_STATE.diffX+","+SMOUSE_STATE.diffY);
    }
    else {
        if(e.target&&e.target.sui) SMOUSE_STATE.source=e.target.sui;
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
    if(e.target){
        if(e.target.sui){
            if(CURRENT_MENU){
                if(e.target.sui != CURRENT_MENU) CURRENT_MENU.close();
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
    if(e.target){
        if(e.target.sui){
            if(CURRENT_MENU){
                if(e.target.sui != CURRENT_MENU) CURRENT_MENU.close();
            }
        }
        else {
            if(CURRENT_MENU){ CURRENT_MENU.close(); }
        }
    }
    else {
        if(CURRENT_MENU){ CURRENT_MENU.close(); }
    }
});
document.addEventListener('drag',function(e){
});
document.addEventListener('dragstart',function(e){
    if(e.target&&e.target.sui) SMOUSE_STATE.source=e.target.sui;
    SMOUSE_STATE.dragging=true;
});
document.addEventListener('dragend', function(e){

});
document.addEventListener('dragover', function(e){

});
document.addEventListener('dragenter', function(e){

});
document.addEventListener('dragleave', function(e){

});
document.addEventListener('drop',function(e){
    event.preventDefault();

});














function sframe(a){return new SFrame(a);}



function simgview(a){return new SImageView(a);}












function SLayout(t){this.type=t;this.C=undefined;};
SLayout.prototype={
    applyLayout:function(c){this.C=c;this.C.setDisplay(this.type);},
    append:function(c,l){this.C.node.appendChild(c);},
    insert:function(i,c,l){this.C.node.insertBefore(c,this.C.node.childNodes[i]);},
    remove:function(c,l){c.parentNode.removeChild(c);},
    clear:function(l){while(this.C.node.hasChildNodes()){this.C.node.removeChild(this.C.node.childNodes[0]);}}
};
function slayout(a){return new SLayout(a);};
function SColumnLayout(n){SLayout.call(this,'flex');this.count=n;};
SColumnLayout.prototype=Object.create(SLayout.prototype,{
    applyLayout:{value:function(c){
        SLayout.prototype.applyLayout.apply(this,[c]);
        this.C.addClass('column-layout');
        this.C.node.style.columnCount=this.count;
   }}
});
SColumnLayout.prototype.constructor=SColumnLayout;
function scolumn(a){return new SColumnLayout(a);};
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
function sflow(a,b){return new SFlowLayout(a,b);};
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
function scard(a,b){return new SCardLayout(a,b);};function scard(a,b){return new SCardLayout(a,b);};
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
function sborder(){return new SBorderLayout();};
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
                for (var a2=1;a2<this.areas[a].length;a2++){tmp2+=' '+this.areas[a][a2];} tmp2+='"';
                tmp+=tmp2+' ';
            };
            this.C.node.style.gridTemplateAreas=tmp;
        }
    },
    append:{value:function(c,l){this.C.node.appendChild(c);c.style.gridArea=l;}}
});
SGridLayout.prototype.constructor=SGridLayout;
function sgrid(a,b,c){return new SGridLayout(a,b,c);};
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
        if(p.display) this.setDisplay(p.display);
        this.node.classList.add('component');
        if(p.id) this.setID(p.id);
        if(p.name) this.setName(p.name);
        if(p.tooltip) this.setToolTip(p.tooltip);
        if(p.uiid) this.setSuiID(p.uiid);
        if(p.locale) this.setLocale(p.locale);
        if(p.class&&p.class.length) this.setClass(p.class);
        if(p.layout) this.setLayout(p.layout);
        if(p.components) this.setComponents(p.components);
        if(p.margin&&p.margin.length){this.setMargin(p.margin);}
        if(p.padding&&p.padding.length){this.setPadding(p.padding);}
        if(p.halign) this.setHorizontalAlign(p.halign);
        if(p.valign) this.setVerticalAlign(p.valign);
        if(p.cursor) this.setCursor(p.cursor);
        if(p.x!==undefined) this.setX(p.x);
        if(p.y!==undefined) this.setY(p.y);
        if(p.z!==undefined) this.setZ(p.z);
        if(p.position) this.setPosition(p.position);
        if(p.width!==undefined) this.setWidth(p.width);
        if(p.height!==undefined) this.setHeight(p.height);
        if(p.minw!==undefined) this.setMinWidth(p.minw);
        if(p.maxw!==undefined) this.setMaxWidth(p.maxw);
        if(p.minh!==undefined) this.setMinHeight(p.minh);
        if(p.maxh!==undefined) this.setMaxHeight(p.maxh);
        if(p.size!==undefined) this.setSize(p.size);
        if(p.draggable!==undefined) this.setDraggable(p.draggable);
        if(p.border) this.setBorder(p.border);
        if(p.background) this.setBackground(p.background);
        if(p.shadow) this.setShadow(p.shadow);
        if(p.color) this.setColor(p.color);
        if(p.event) this.setEventListener(p.event);
        if(p.available!==undefined) this.setAvailable(p.available);
        if(p.visible!==undefined) this.setVisible(p.visible);
    },
    setDisplay:function(d){this.display=d;if(d){this.node.style.display=d;}return this;},
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
        else if(this.node.style.display!=='none'){
            this.display=this.node.style.display;
            this.node.style.display='none';
        }
        return this;
    },
    setAttribute:function(a){for(var k in a){this.node[k]=a[k];} return this;},
    setAvailable:function(a){this.node.disabled=!a; return this;},
    setMainClass:function(c){this.node.classList.replace(this.node.classList[0],c); return this;},
    setClass:function(c){for (var i=0;i<c.length;i++){this.addClass(c[i]);} return this;},
    hasClass:function(c){return this.node.classList.contains(c);},
    addClass:function(c){this.node.classList.add(c); return this;},
    removeClass:function(c){if(this.node.classList.contains(c)) this.node.classList.remove(c); return this;},
    componentAt:function(i){return this.components[i];},
    setComponents:function(c){
        if(c instanceof Array&&c.length) for(var i=0;i<c.length;i++){this.add(c[i]);}
        else {for(var e in c){for(var i=0;i<c[e].length;i++){this.add(c[e][i],e);}}}
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
        this.node.style.marginTop=parseValue(margin[0]);
        this.node.style.marginRight=parseValue(margin[1]);
        this.node.style.marginBottom=parseValue(margin[2]);
        this.node.style.marginLeft=parseValue(margin[3]);
    },
    setMarginT:function(margin){this.node.style.marginTop=margin;},
    setMarginB:function(margin){this.node.style.marginBottom=margin;},
    setMarginL:function(margin){this.node.style.marginLeft=margin;},
    setMarginR:function(margin){this.node.style.marginRight=margin;},
    setPadding:function(padding){
        this.node.style.paddingTop=parseValue(padding[0]);
        this.node.style.paddingRight=parseValue(padding[1]);
        this.node.style.paddingBottom=parseValue(padding[2]);
        this.node.style.paddingLeft=parseValue(padding[3]);
    },
    setHorizontalAlign:function(align){this.node.style.textAlign=align; return this;},
    setVerticalAlign:function(align){ this.node.style.verticalAlign=align; return this;},
    setCursor:function(cursor){this.node.style.cursor=cursor; return this;},
    setX:function(x){this.node.style.left=parseValue(x); return this;},
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
    setWidth:function(w){this.node.style.width=parseValue(w);return this;},
    setMinWidth:function(w){this.node.style.minWidth=parseValue(w);return this;},
    setMaxWidth:function(w){this.node.style.maxWidth=parseValue(w);return this;},
    setHeight:function(h){this.node.style.height=parseValue(h);return this;},
    setMinHeight:function(h){this.node.style.minHeight=parseValue(h);return this;},
    setMaxHeight:function(h){this.node.style.maxHeight=parseValue(h);return this;},
    setSize:function(size){this.node.style.fontSize=parseValue(size);},
    setDraggable:function(d){this.node.draggable=d;},
    setBorder:function(border){
        if(border.style) this.node.style.borderStyle=border.style;
        if(border.width) this.node.style.borderWidth=parseValue(border.width);
        if(border.color) this.node.style.borderColor=border.color;
        if(border.radius) this.node.style.borderRadius=parseValue(border.radius);
        if(border.top){
            var tborder=border.top;
            if(tborder.style) this.node.style.borderTopStyle=tborder.style;
            if(tborder.width) this.node.style.borderTopWidth=parseValue(tborder.width);
            if(tborder.color) this.node.style.borderTopColor=tborder.color;
            if(tborder.radius) this.node.style.borderTopRadius=parseValue(tborder.radius);
       }
        if(border.bottom){
            var bborder=border.bottom;
            if(bborder.style) this.node.style.borderBottomStyle=bborder.style;
            if(bborder.width) this.node.style.borderBottomWidth=parseValue(bborder.width);
            if(bborder.color) this.node.style.borderBottomColor=bborder.color;
            if(bborder.radius) this.node.style.borderBottomRadius=parseValue(bborder.radius);
       }
        if(border.left){
            var lborder=border.left;
            if(lborder.style) this.node.style.borderLeftStyle=lborder.style;
            if(lborder.width) this.node.style.borderLeftWidth=parseValue(lborder.width);
            if(lborder.color) this.node.style.borderLeftColor=lborder.color;
            if(lborder.radius) this.node.style.borderLeftRadius=parseValue(lborder.radius);
       }
        if(border.right){
            var rborder=border.right;
            if(rborder.style) this.node.style.borderRightStyle=rborder.style;
            if(rborder.width) this.node.style.borderRightidth=parseValue(rborder.width);
            if(rborder.color) this.node.style.borderRightColor=rborder.color;
            if(rborder.radius) this.node.style.borderRightRadius=parseValue(rborder.radius);
       }
        return this;
    },
    setBackground:function(back){
        if(back.color){this.node.style.background=back.color;}
        else if(back.image){this.node.style.backgroundImage=back.image;}
        return this;
    },
    setShadow:function(shadow){this.node.style.boxShadow=shadow; return this;},
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
    setEventListener:function(l){
        for(var i=0;i<l.length;i++){
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
function suic(a,b){return new SUIComponent(a,b);};
function SSpacer(){SUIComponent.call(this,'div',{class:['sspacer','auto','unresponsible']});};
SSpacer.prototype=Object.create(SUIComponent.prototype,{
    initNode:{value:function(p){
        SUIComponent.prototype.initNode.apply(this,[p]);
        this.setSuiID('spacer').setMainClass('sspacer');
    }}
});
SSpacer.prototype.constructor=SSpacer;
function sspace(){return new SSpacer();};
function SBar(d,p){p=propOverride(p,{style:'solid',width:0.25,color:'dimgray'});
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
function sbar(a,b){return new SBar(a,b);};
function SView(p){p=propOverride(p,{});SUIComponent.call(this,'div',p);};
SView.prototype=Object.create(SUIComponent.prototype, {
    initNode:{value:function(p){
        SUIComponent.prototype.initNode.apply(this,[p]);
        this.setSuiID('view').setMainClass('sview');
    }}
});
SView.prototype.constructor=SView;
function sview(a){return new SView(a);}
function SPanel(p){p=propOverride(p,{});SUIComponent.call(this,'div',p);};
SPanel.prototype=Object.create(SUIComponent.prototype, {
    initNode:{value:function(p){
        SUIComponent.prototype.initNode.apply(this,[p]);
        this.setSuiID('panel').setMainClass('spane;');
    }}
});
SPanel.prototype.constructor=SPanel;
function spanel(a){return new SPanel(a);}




function SFrame(p){
    p=propOverride(p,{rorate:false,resize:true,move:true,header:TOP,hdrag:true,close:true,minimize:true,maximize:true});
    p.layout=sflow(VERTICAL);
    SUIComponent.call(this,'div',p);
};
SFrame.prototype=Object.create(SUIComponent.prototype, {
    initNode:{
        value:function(p){
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
    add:{value:function(u){this.components.push(u); this.content.add(u); return this;}},
    minimize:{value:function(){if(this.min) this.content.setVisible(true); else this.content.setVisible(false); this.min=!this.min;}},
    move:{value:function(dx,dy){this.setX(this.X()+dx);this.setY(this.Y()+dy);}},
});
SFrame.prototype.constructor=SFrame;
function SWindow(p){
    p=propOverride(p,{title:'unnamed',resize:true,move:true,header:TOP,close:true,minimize:true,maximize:true});
    SUIComponent.call(this,'div',p);
};
SWindow.prototype=Object.create(SUIComponent.prototype, {
    initNode:{
        value:function(p){
            SUIComponent.prototype.initNode.apply(this,[p]);
            this.header=sview({
                layout:sflow((p.header==LEFT||p.header==RIGHT)?VERTICAL:HORIZONTAL),
                class:['window-header']
            });
            this.title=slabel(title,{class:['window-title','unresponsible','unselectable']});

            this.content=sview();


        }
    },
    add:{value:function(u){this.components.push(u); this.content.add(u); return this;}},
    close:{value:function(){this.setVisible(false);}},
    minimize:{value:function(){if(this.min) this.content.setVisible(true); else this.content.setVisible(false); this.min=!this.min;}},
    move:{value:function(dx,dy){this.setX(this.X()+dx);this.setY(this.Y()+dy);}},
});
SWindow.prototype.constructor=SWindow;

function SDialog(p){
    p=propOverride(p,{mode:'generic',modal:true,message:'',content:null});
    this.mode=p.mode;this.modal=p.modal;
    if(p.mode==='generic') {
        p.layout=sflow(VERTICAL);p.class=['align-center'];
        this.content=p.content?p.content:spanel();
        SUIComponent.call(this,'div',p);
    }
    else {this.result=null;this.message=p.message;}
};
SDialog.prototype=Object.create(SUIComponent.prototype, {
    initNode:{value:function(p){
        SUIComponent.prototype.initNode.apply(this,[p]);
        this.setSuiID('dialog').setMainClass('sscreen').add(this.content).setModal(p.modal).setVisible(false);
        const Cls=this;
        this.content.setMainClass('sdialog');
        this.content.node.onclick=function(e){e.stopPropagation();};
        this.node.onclick=function(e){Cls.hide();};
    }},
    show:{value:function(){
        if(this.mode==='alert') alert(this.message);
        else if(this.mode==='confirm') this.result=confirm(this.message);
        else if(this.mode==='prompt') this.result=prompt(this.message);
        else {this.setVisible(true); this.content.addClass('show');}
    }},
    hide:{value:function(){
        if(this.mode==='generic'){this.setVisible(false);this.content.removeClass('show');}
    }},
    setModal:{value:function(m){
        if(this.mode==='generic'){this.modal=m;if(m){this.addClass('modal');}else{this.removeClass('modal');}} return this;
    }},
    setContent:{value:function(c){
        c.setMainClass('sdialog');c.node.onclick=function(e){e.stopPropagation();};
        this.remove(this.content);this.content=c;this.add(this.content);return;
    }}
});
SDialog.prototype.constructor=SDialog;
function sdialog(a){return new SDialog(a);}
function SBox(p){p=propOverride(p,{label:'',content:null});SUIComponent.call(this,'fieldset',p);};
SBox.prototype=Object.create(SUIComponent.prototype, {
    initNode:{
        value:function(p){
            SUIComponent.prototype.initNode.apply(this,[p]);
            this.label=slabel(p.label);this.legend=suic('legend');this.legend.add(this.label);
            this.content=p.content?p.content:sview();
            this.setSuiID('box').setMainClass('sbox').add(this.legend).add(this.content);
        }
    },
    title:{value:function(){return this.label.text();}},
    setTitle:{value:function(t){this.label.setText(t); return this;}}
});
SBox.prototype.constructor=SBox;
function sbox(a){return new SBox(a);}
function SExpandView(p){
    p=propOverride(p,{style:DEFAULT,direction:BOTTOM,expand:false,label:['show','hide'],icon:[TRI_CHARS[RIGHT],TRI_CHARS[BOTTOM]],content:null});
    this.direction=p.direction;this.style=p.style;
    if(p.style){
        if(p.direction<2) p.layout=sflow(VERTICAL);
        else p.layout=sflow(HORIZONTAL);
        SUIComponent.call(this,'div',p);
    }
    else SUIComponent.call(this,'details',p);
};
SExpandView.prototype=Object.create(SUIComponent.prototype, {
    initNode:{
        value:function(p){
            SUIComponent.prototype.initNode.apply(this,[p]);
            const Cls=this;
            this.setSuiID('exview').setMainClass('sexview');
            this.labels=p.label;this.icons=null;this.header=null;this.controler=null;
            this.label=slabel('',{class:['unselectable','unresponsible']});
            this.label.setMainClass('exview-label');
            if(p.content) this.content=p.content;
            else this.content=sview();
            this.content.setMainClass('exview-content');
            if(p.style){
                this.header=sview({layout:sflow(HORIZONTAL),class:['exview-header'],event:[{type:'click',func:function(e){Cls.setState(!Cls.state);}}]});
                this.controler=spanel({class:['exview-expander','unresponsible']});
                this.icons=[parseImage(p.icon[0]),parseImage(p.icon[1])];
                if(this.direction<2) this.controler.addClass('horizontal');
                else {
                    this.label.addClass('vtext');
                    this.controler.addClass('vertical');
                }
                if(this.expand===0||this.expand===2) this.addClass('reverse');
                this.header.add(this.controler).add(this.label);
            }
            else {
                this.header=suic('summary');
                this.header.add(this.label);
                this.node.ontoggle=function(e){Cls.state=Cls.node.open; Cls.update();}
            }
            this.add(this.header).add(this.content).setState(p.state);
        }
    },
    expand:{value:function(){this.setState(true); return this;}},
    collapse:{value:function(){this.setState(false); return this;}},
    update:{value:function(){
        if(this.state) this.addClass('open');
        else this.removeClass('open');
        this.label.setText(this.labels[this.state?1:0]);
        if(this.controler){
            this.controler.clear();
            this.controler.add(this.icons[this.state?1:0]);
        }
    }},
    setState:{value:function(s){
        this.state=s;
        if(this.style) this.content.setVisible(s);
        else this.node.open=this.s;
        this.update(); return this;
    }},
    setContent:{value:function(c){
        c.setMainClass('exview-content');this.remove(this.content);this.content=c;this.add(this.content);
    }}
});
SExpandView.prototype.constructor=SExpandView;
function sexview(a){return new SExpandView(a);}



function STab(p){
    p=propOverride(p,{direction:HORIZONTAL,content:null,icon:null,label:'tab',close:true,draggable:true});
    p.layout=sflow(p.direction);SUIComponent.call(this,'div',p);
}
STab.prototype=Object.create(SUIComponent.prototype, {
    initNode:{value:function(p){
        SUIComponent.prototype.initNode.apply(this,[p]);
        const Cls=this;this.active=false;this.icon=null;this.content=null;
        if(p.icon) this.icon=parseImage(p.icon)
        else this.icon=slabel('',{class:['unselectable','unresponsible']});
        this.icon.addClass('tab-icon');
        this.label=slabel(p.label,{class:['tab-title','unselectable']});
        this.node.onclick=function(e){Cls.parent.select(Cls);};
        this.closer=spanel({class:['tab-closer']});
        this.closer.node.onclick=function(e){e.stoppagation();Cls.parent.close(Cls);}
        if(!p.close) this.closer.setVisible(false);
        this.setSuiID('tab').setMainClass('stab').add(sspace()).add(this.icon).add(this.title).add(sspace()).add(this.closer).setContent(p.content);
    }},
    setActive:{value:function(a){
        if(a){this.addClass('active');}else{this.removeClass('active');}
        this.active=a;return this;
    }},
    setContent:{value:function(c){this.content=c;if(c){this.content.setMainClass('tab-content');}return this;}},
    title:{value:function(){return this.label.text();}},
    setTitle:{value:function(t){this.label.setText(t);return this;}}
});
STab.prototype.constructor=STab;
function stab(a){return new STab(a);}
function STabViewHeader(d){
    SUIComponent.call(this,'div',{layout:sflow(d)});
};
STabViewHeader.prototype=Object.create(SUIComponent.prototype, {
    initNode:{
        value:function(p){
            SUIComponent.prototype.initNode.apply(this,[p]);
            var Cls=this;this.setSuiID('tabheader').setMainClass('stab-header');
        }
    }
});
STabViewHeader.prototype.constructor=STabViewHeader;
function STabView(p){
    p=propOverride(p,{hpos:TOP,direction:VERTICAL,tabs:[],selected:0});
    p.layout=sflow(p.direction);SUIComponent.call(this,'div',p);
};
STabView.prototype=Object.create(SUIComponent.prototype, {
    initNode:{
        value:function(p){
            SUIComponent.prototype.initNode.apply(this,[p]);
            this.tabs=[];
            this.selected=null;
            this.header=new STabViewHeader(p.direction==HORIZONTAL?VERTICAL:HORIZONTAL);
            this.content=sview({class:['tabview-content']});
            if(p.tabs.length){
                for (var i=0;i<p.tabs.length;i++){
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
        value:function(p){
            var tab=new STab(p);
            this.tabs.push(tab);
            this.header.addTab(tab);
            return this;
        }
    },
    indexOf:{value:function(t){return this.tabs.indexOf(t);}},
    select:{
        value:function(t){
            if(this.selected){
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
        value:function(i){
            if(i<this.tabs.length) this.select(this.tabs[i]);
        }
    },
    close:{value:function(t){
        const i=this.indexOf(t);
        this.header.removeTab(t);
        this.tabs.splice(i, 1);
        if(t.active){
            this.content.clear();
            if(i===this.tabs.length) i--;
            this.selectIndex(i);
        }
    }}
});
STabView.prototype.constructor=STabView;
function stabview(a){return new STabView(a);}
function SSplitter(p,s,d){
    this.primary=p;this.secondary=s;this.direction=d;
    this.border=null;this.edge=(p==null||s==null);
    SUIComponent.call(this,'div',{});
};
SSplitter.prototype=Object.create(SUIComponent.prototype, {
    initNode:{
        value:function(){
            SUIComponent.prototype.initNode.apply(this,[p]);
            this.setSuiID('splitter').setMainClass('ssplitter').addClass(this.direction==HORIZONTAL?'horizontal':'vertical');
        }
    },
    update:{value:function(e){
        if(this.direction==HORIZONTAL){
            if(this.primary){
                if(this.primary.width()+SMOUSE_STATE.diffX<0) this.primary.setWidth(0);
                else this.primary.setWidth(this.primary.width()+SMOUSE_STATE.diffX);
            }
            if(this.secondary){
                if(this.secondary.X()+SMOUSE_STATE.diffX<this.primary.X()) this.secondary.setX(this.primary.X());
                else this.secondary.setX(this.secondary.X()+SMOUSE_STATE.diffX);
            }
        }
        else {
            if(this.primary){
                if(this.primary.height()+SMOUSE_STATE.diffY<0) this.primary.setHeight(0);
                else this.primary.setHeight(this.primary.height()+SMOUSE_STATE.diffY);
            }
            if(this.secondary){
                if(this.secondary.Y()+SMOUSE_STATE.diffY<this.primary.Y()) this.secondary.setY(this.primary.Y());
                else this.secondary.setY(this.secondary.Y()+SMOUSE_STATE.diffY);
            }
        }
    }}
});
SSplitter.prototype.constructor=SSplitter;
function ssplitter(a,b,c){return new SSplitter(a,b,c);}
function SSplitView(d,p){
    p=propOverride(p,{ratio:1.0,primary:null,secondary:null});
    this.direction=d;p.layout=sflow(d);
    SUIComponent.call(this,'div',p);
}
SSplitView.prototype=Object.create(SView.prototype, {
    initNode:{
        value:function(p){
            SUIComponent.prototype.initNode.apply(this,[p]);
            if(this.direction===HORIZONTAL) this.primary=sview({width:(50.0*p.ratio)+'%',class:['split-primary']});
            else this.primary=sview({height:(50.0*p.ratio)+'%',class:['split-primary']});
            this.secondary=sview({class:['split-secondary']});
            if(p.primary) this.primary.add(p.primary);
            if(p.secondary) this.secondary.add(p.secondary);
            this.splitter=ssplitter(this.primary,this.secondary,this.direction);
            this.setSuiID('splitview').setMainClass('split-view').add(this.primary).add(this.splitter).add(this.secondary);
        }
    },
    setPrimaryView:{value:function(v){this.primary.clear();this.primary.add(v);return this;}},
    setSecondaryView:{value:function(v){this.secondary.clear();this.secondary.add(v);return this;}}
});
SSplitView.prototype.constructor=SSplitView;
function ssplitview(a,b){return new SSplitView(a,b);}
function SWebView(p){p=propOverride(p,{src:''});SUIComponent.call(this,'iframe',p);}
SWebView.prototype=Object.create(SUIComponent.prototype, {
    initNode:{
        value:function(p){
            SUIComponent.prototype.initNode.apply(this, [ p ]);
            this.setSuiID('webview').setMainClass('swebview').setAttribute({frameBorder:'0'}).setSource(p.src);
        }
    },
    source:{value:function(){return this.node.contentWindow.location;}},
    setSource:{value:function(src){this.node.src=src;return this;}}
});
SWebView.prototype.constructor=SWebView;
function swebview(a){return new SWebView(a);}



function SImageView(p){p=propOverride(p,{src:'',alt:'',width:null,height:null});SUIComponent.call(this,'img',p);};
SImageView.prototype=Object.create(SUIComponent.prototype, {
    initNode:{
        value:function(p){
            SUIComponent.prototype.initNode.apply(this,[p]);
            const Cls=this;this.oriWidth=0;this.oriHeight=0;this.image=new Image();
            this.image.onload=function(){Cls.oriWidth=Cls.image.width;Cls.oriHeight=Cls.image.height;};
            this.setSuiID('image').setMainClass('simage').setSource(p.src).setAlt(p.alt).setWidth(p.width).setHeight(p.height);
        }
    },
    source:{value:function(){return this.node.src;}},
    setSource:{value:function(s){this.image.src=s;this.node.src=s;return this;}},
    altText:{value:function(){return this.node.alt;}},
    setAlt:{value:function(t){this.node.alt=t; return this;}},
    setWidth:{value:function(w){if(w){this.node.width=w;}return this;}},
    setHeight:{value:function(h){if(h){this.node.height=h;}return this;}}
});
SImageView.prototype.constructor=SImageView;

function SSvgCanvas(p){

}
function SFigure(p){
    if(p == null) p={};
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

function SCanvas(p){
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
    setPainter:{value:function(p){
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
    arc:{value:function(x,y,r,s,e,d){this.context.arc(x,y,r,s,e,d);}},
    drawRect: {value:function(x,y,w,h,p){
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
        this.context.arcTo(x+width, y+height, x+width, y+height - radius, radius);
        this.context.lineTo(x+width, y+radius);
        this.context.arcTo(x+width, y, x+width - radius, y, radius);
        this.context.lineTo(x+radius, y);
        this.context.arcTo(x, y, x, y+radius, radius);
        this.context.fill();
        this.context.stroke();
    }},
    drawEllipse: {value:function(x,y,w,h,t,p){
        if(p) this.setPainter(p);

    }},
    drawArc: {value:function(x,y,w,h,b,e,t,p){
        if(p) this.setPainter(p);

    }},
    drawPolygon: {value:function(x,y,n,t,p){
        if(p) this.setPainter(p);

    }},
    erase: {value:function(x,y,w,h){
        this.context.clearRect(x,y,w,h);
    }}

});
SCanvas.prototype.constructor=SCanvas;


function SColorPicker(p){
    if(p == null) p={};
    SUIComponent.call(this, 'input', p);
};
SColorPicker.prototype=Object.create(SUIComponent.prototype, {
    initNode:{
        value:function(p){
            SUIComponent.prototype.initNode.apply(this, [ p ]);
            this.className='colorpicker';
            this.addClass('scolor-picker');
            this.node.type='color';
        }
    }
});
SColorPicker.prototype.constructor=SColorPicker;




////////////////



function SListItem(p){p=propOverride(p,{content:null});SUIComponent.call(this,'li',p);};
SListItem.prototype=Object.create(SUIComponent.prototype,{
    initNode:{value:function(p){
        SUIComponent.prototype.initNode.apply(this,[p]);
        this.setSuiID('listitem').setMainClass('sli-item');
        if(p.content) this.node.innerHTML=p.content;
    }}
});
SListItem.prototype.constructor=SListItem;
function slistitem(a){return new SListItem(a);};
function SList(p){p=propOverride(p,{type:'ol',prefix:'decimal',items:[]});SUIComponent.call(this,p.type,p);};
SList.prototype=Object.create(SUIComponent.prototype,{
    initNode:{value:function(p){
        SUIComponent.prototype.initNode.apply(this,[p]);
        this.setSuiID('list').setMainClass('slist');
        this.node.style.listStyle=p.prefix;
        for (var i=0;i<p.items.length;i++){if(p.items[i].suiid){this.add(p.items[i]);}else{this.add(slistitem(p.items[i]));}}
    }}
});
SList.prototype.constructor=SList;
function slist(a){return new SList(a);};
function SListView(p){
    p=propOverride(p,{direction:VERTICAL,items:[]});
    p.layout=sflow(p.direction);
    SUIComponent.call(this,'div',p);
};
SListView.prototype=Object.create(SUIComponent.prototype,{
    initNode:{value:function(p){
        SUIComponent.prototype.initNode.apply(this,[p]);
        this.setSuiID('listview').setMainClass('slist-view');
        for(var i=0;i<p.items.length;i++){this.add(p.items[i]);}
    }}
});
SListView.prototype.constructor=SListView;
function slistview(a){return new SListView(a);};
function SCardView(p){
    p=propOverride(p,{direction:HORIZONTAL,items:[]});
    p.layout=scard(p.direction);
    SUIComponent.call(this,'div',p);
};
SCardView.prototype=Object.create(SUIComponent.prototype,{
    initNode:{value:function(p){
        SUIComponent.prototype.initNode.apply(this,[ p ]);
        this.setSuiID('cardview').setMainClass('scard-view');
        for(var i=0;i<p.items.length;i++){this.add(p.items[i]);}
    }}
});
SCardView.prototype.constructor=SCardView;
function scardview(a){return new SCardView(a);};
function SCellRenderer(){};
SCellRenderer.prototype={
    render:function(c,p){
        p.class=['unselectable'];
        c.add(suic('p',p));
        if(p.value) c.componentAt(0).node.innerHTML=p.value;
        c.componentAt(0).node.ondblclick=function(e){
            c.componentAt(0).removeClass('unselectable');
            if(c.table.editable) {
                c.componentAt(0).node.contentEditable=true;
                c.componentAt(0).addClass('editing');
            }
        };
        c.componentAt(0).node.onmouseleave=function(e){
            c.componentAt(0).node.contentEditable=false;
            c.componentAt(0).removeClass('editing');
            c.componentAt(0).addClass('unselectable');
        };
    },
    setValue:function(c,v){c.componentAt(0).node.innerHTML=v;},
    getValue:function(c){return c.componentAt(0).node.innerHTML;},
};
function SCheckCellRenderer(){};
SCheckCellRenderer.prototype={
    render:function(c,p){
        p.state=p.value;p.available=c.table.editable;
        c.add(scheck(p));
    },
    setValue:function(c,v){c.componentAt(0).setState(v);},
    getValue:function(c){return c.componentAt(0).state;}
};
function SSelectCellRenderer(i){this.items=i;}
SSelectCellRenderer.prototype={
    render:function(c,p){
        p.items=[];p.available=c.table.editable;
        for(var i=0;i<this.items.length;i++){p.items.push({label:this.items[i],value:this.items[i]});}
        c.add(sselect(p));
        c.componentAt(0).selectValue(p.value);
        c.componentAt(0).setMainClass('cell-select');
    },
    setValue:function(c,v){c.componentAt(0).selectAt(v);},
    getValue:function(c){return c.componentAt(0).selected.value();},
    setEditable:function(c,e){ c.componentAt(0).setAvailable(e); }
}
function SButtonCellRenderer(l){this.label=l;}
SButtonCellRenderer.prototype={
    render:function(c,p){
        p.label=this.label;p.action=p.value;
        c.add(sbutton(p));
        c.componentAt(0).setMainClass('cell-button');
    },
    setValue:function(c,v){c.componentAt(0).setAction(v);},
    getValue:function(c){return c.componentAt(0).action;},
}
function SUICellRenderer(){}
SUICellRenderer.prototype={
    render:function(c,p){c.add(p.value);},
    setValue:function(c,v){c.clear().add(v);},
    getValue:function(c){return c.componentAt(0);},
}
function STableColumn(t,p){
    p=propOverride(p,{label:'column',renderer:new SCellRenderer(),items:null,sort:false,search:false});
    this.table=t;this.cindex=0;this.renderer=p.renderer;
    SUIComponent.call(this,'th',p);
};
STableColumn.prototype=Object.create(SUIComponent.prototype,{
    initNode:{
        value:function(p){
            SUIComponent.prototype.initNode.apply(this,[p]);
            this.sort=p.sort;this.search=p.search;
            this.header=sview({layout:sflow(HORIZONTAL)});
            this.label=slabel(p.label,{class:['column-label']});
            this.ctitle=sview({layout:sflow(HORIZONTAL)});
            this.sorter=sview({layout:sflow(VERTICAL),class:['column-sorter']});
            this.sorter.add(slabel(TRI_CHARS[TOP],{class:['asc','unselectable']}))
            .add(slabel(TRI_CHARS[BOTTOM],{class:['desc','unselectable']}));
            this.ctitle.add(sspace()).add(this.label).add(sspace());
            this.header.add(this.ctitle).add(this.sorter);
            this.setSuiID('tablecolumn').setMainClass('stable-column').add(this.header)
            .setSortable(p.sort).setSearchable(p.search);
        }
    },
    setLabel:{value:function(t){this.label.setText(t);return this;}},
    setSortable:{value:function(s){this.sort=s;this.sorter.setVisible(s); return this;}},
    setSearchable:{value:function(s){this.search=s; return this;}},
    setRenderer:{value:function(r){this.renderer=r; return this;}}
});
STableColumn.prototype.constructor=STableColumn;
function stablecolumn(a,b){return new STableColumn(a,b);};
function STableRow(t,p){
    p=propOverride(p,{values:[]})
    this.table=t;this.rindex=0;this.cells=[];
    SUIComponent.call(this,'tr',p);
};
STableRow.prototype=Object.create(SUIComponent.prototype,{
    initNode:{
        value:function(p){
            SUIComponent.prototype.initNode.apply(this,[p]);
            this.setSuiID('tablerow').addClass('stable-row').addCell({value:''});
            for(var c=1;c<this.table.columns.length;c++) {this.addCell({value:p.values[c-1]});}
            this.cells[0].setMainClass('row-header').setVisible(this.table.showRowHeader);
        }
    },
    addCell:{value:function(c){
        if(c.suiid) {this.cells.push(c);this.add(c);}
        else {
            var c_=stablecell(this.table,this,this.table.columns[this.cells.length],c);
            this.cells.push(c_);this.add(c_);
        }
        return this;
    }},
    cellAt:{value:function(i){return this.cells[i];}}
});
STableRow.prototype.constructor=STableRow;
function stablerow(a,b){return new STableRow(a,b);};
function STableCell(t,r,c,p){
    p=propOverride(p,{value:null});
    this.table=t;this.row=r;this.column=c;
    SUIComponent.call(this,'td',p);
};
STableCell.prototype=Object.create(SUIComponent.prototype,{
    initNode:{
        value:function(p){
            SUIComponent.prototype.initNode.apply(this,[p]);
            this.setSuiID('tablecell').setMainClass('stable-cell');
            this.column.renderer.render(this,p);
        }
    },
    setValue:{value:function(v){this.column.renderer.setValue(this,v);}},
    value:{value:function(){this.column.renderer.getValue(this);}}
});
STableCell.prototype.constructor=STableCell;
function stablecell(a,b,c,d){return new STableCell(a,b,c,d);};
function STable(p){
    p=propOverride(p,{columns:null,rows:null,load:null,editable:false,selectType:'cell',showRowHeader:false});
    SUIComponent.call(this,'table',p);
};
STable.prototype=Object.create(SUIComponent.prototype,{
    initNode:{
        value:function(p){
            SUIComponent.prototype.initNode.apply(this,[p]);
            const Cls=this;
            this.header=suic('thead',{class:['table-header']});
            this.body=suic('tbody',{class:['table-body']});
            this.footer=suic('tfoot',{class:['table-footer']})
            this.colview=suic('tr',{class:['column-header']});
            this.header.add(this.colview);
            this.editable=p.editable;this.showRowHeader=p.showRowHeader;this.selectType=p.selectType;
            this.columns=[];this.rows=[];
            this.setSuiID('table').setMainClass('stable').add(this.header).add(this.body).add(this.footer).addColumn({label:''});
            if(p.columns&&p.columns.length){
                for(var c=0;c<p.columns.length;c++){ this.addColumn(p.columns[c]); }
            }
            if(p.rows&&p.rows.length){
                for(var r=0;r<p.rows.length;r++){ this.addRow(p.rows[r]); }
            }
            if(p.load) {
                var r=sget(p.load,'json');
                r.then(function(d){Cls.setData(d);},function(e){console.error(e);});
            }
            this.columns[0].setMainClass('table-corner');
            this.setRowHeader(p.showRowHeader);
        }
    },
    columnAt:{value:function(i){return this.columns[i];}},
    columnCount:{value:function(){return this.columns.length;}},
    addColumn:{value:function(c){
        if(c.suiid){this.columns.push(c);this.colview.add(c);}
        else {var c_=stablecolumn(this,c);this.columns.push(c_);this.colview.add(c_);}
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
        this.colview.clear();this.clolumns=[];return this;
    }},
    rowAt:{value:function(i){return this.rows[i];}},
    rowCount:{value:function(){return this.rows.length;}},
    addRow:{value:function(r){
        if(r.suiid) {this.rows.push(r);this.body.add(r);}
        else {
            var r_=null;
            if(r instanceof Array) r_=stablerow(this,{values:r});
            else r_=stablerow(this,r);
            this.rows.push(r_);this.body.add(r_);
        }
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
    cellAt:{value:function(i,j){return this.rows[i].cellAt(j);}},
    setCell:{value:function(r,c,v){this.columns[c].renderer.setValue(this.rows[r].cellAt(c),v);return this;}},
    setRowHeader:{value:function(b){this.showRowHeader=b;this.columns[0].setVisible(b);}},
});
STable.prototype.constructor=STable;
function stable(a){return new STable(a);};

function STableView(p){
    p=propOverride(p,{table:null,paging:true,count:20,current:1,});
    SUIComponent.call(this,'table',p);
};
STableView.prototype=Object.create(SUIComponent.prototype,{
    initNode:{
        value:function(p){
            SUIComponent.prototype.initNode.apply(this,[ p ]);
            this.header=null;this.main=null;this.footer=null;


        }
    },
    currentPage:{value:function(){return this.current;}},
    pageCount:{value:function(){ return null;}},
});
STableView.prototype.constructor=STableView;
function stableview(a){return new STableView(a);};

function STreeNode(p){
    p=propOverride(p,{layout:sflow(VERTICAL),style:NORMAL_TREE,label:'item',leaf:true,state:false,icon:null,children:[]});
    this.style=p.style;this.leaf=p.leaf;this.rootview=null;
    SUIComponent.call(this,'div',p);
};
STreeNode.prototype=Object.create(SUIComponent.prototype,{
    initNode:{
        value:function(p){
            SUIComponent.prototype.initNode.apply(this,[p]);
            const Cls=this;
            this.layer=0;
            this.state=false;
            this.icons=null;
            this.action=p.action;
            this.space=spanel();
            this.hpane=spanel({layout:sflow(HORIZONTAL),class:['tnode-header']});
            this.label=slabel(p.label,{class:['tnode-label','unresponsible','unselectable']});
            this.toggle=sview({class:['tnode-icon','unresponsible','unselectable']});
            if(this.style===NORMAL_TREE){
                this.setMainClass('stree-node');
                if(p.icon) this.icons=[parseImage(p.icon[0]),parseImage(p.icon[1]),parseImage(p.icon[2])];
                else this.icons=[parseImage(''),parseImage(TRI_CHARS[RIGHT]),parseImage(TRI_CHARS[BOTTOM])];
            }
            else if(this.style===FILE_TREE){
                this.setMainClass('sfile-node');
                if(p.icon) this.icons=[parseImage(p.icon[0]),parseImage(p.icon[1]),parseImage(p.icon[2])];
                else this.icons=[parseImage('icon:insert_drive_file'),parseImage('icon:folder'),parseImage('icon:folder_open')];
            }
            if(this.leaf) this.toggle.add(this.icons[0]);
            this.hpane.add(this.space).add(this.toggle).add(this.label);
            this.hpane.node.onclick=function(e){
               if(!Cls.leaf) Cls.setState(!Cls.state);
               if(Cls.action) Cls.action(e);
            }
            this.children=sview({layout:sflow(VERTICAL),class:['tree-children']});
            if(!this.leaf&&p.children.length){
                for (var c=0;c<p.children.length;c++){this.addNode(p.children[c]);}
            }
            this.setSuiID('treenode').add(this.hpane).add(this.children).setState(p.state);
        }
    },
    setRootView:{value:function(v){this.rootview=v;return this;}},
    setLayer:{value:function(l){
        this.layer=l;this.space.setWidth(l+'rem');
        for(var c=0;c<this.children.components.length;c++){this.children.components[c].setLayer(l+1);}return this;
    }},
    addNode:{value:function(n){
        if(n.suiid) {
            n.setRootView(this.rootview);
            n.setLayer(this.layer+1);
            this.children.add(n);
        }
        else this.addNode(streenode(n));
        if(this.leaf) this.leaf=false;        
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
function streenode(a){return new STreeNode(a);};
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
            this.root.setRootView(this);
            this.root.setLayer(p.showRoot?0:-1);
            this.setSuiID('treeview').setMainClass('stree-view').update();
        }
    },
    update:{value:function(){
        this.clear();
        if(this.root){
            if(this.showRoot) this.add(this.root);
            else this.add(this.root.children);
        }
        else return;
    }},
    setRoot:{value:function(r){this.root=r;this.update();return this;}}
});
STreeView.prototype.constructor=STreeView;
function streeview(a){return new STreeView(a);};

function SMenuItem(p){
    p=propOverride(p,{style:S_LABEL|S_ICON|S_MODIFIER,state:false,label:'',icon:null,direction:HORIZONTAL,expand:NONE,shortcut:null,submenu:false,items:null,action:null});
    p.layout=sflow(p.direction);
    SUIComponent.call(this,'div',p);
};
SMenuItem.prototype=Object.create(SUIComponent.prototype, {
    initNode: {
        value: function(p){
            SUIComponent.prototype.initNode.apply(this,[p]);
            const Cls=this;this.style=p.style;this.active=false;this.state=null;this.expand=p.expand;//this.focused=false;
            this.toggle=slabel('',{class:['menu-toggle','unselectable','unresponsible']});
            this.label=null;this.icon=null;this.modify=null;this.submenu=null;
            if(p.icon) this.icon=parseImage(p.icon).addClass('menu-icon');
            else this.icon=slabel('',{class:['menu-icon','unselectable','unresponsible']});
            this.label=slabel(p.label,{class: ['menu-label','unselectable','unresponsible']});
            if(this.hasClass('vertical')) this.label.addClass('vtext');
            if(p.submenu){
                this.submenu=p.submenu;this.submenu.setVisible(false);
                this.modify=slabel(TRI_CHARS[p.expand],{class: ['menu-expand','unselectable','unresponsible']});
                this.add(this.submenu);
            }
            else if((this.style&S_MODIFIER)&&p.shortcut){
                var sc=new SShortCut(p.shortcut,p.action);
                SC_MANAGER.register(sc);
                this.modify=slabel(sc.toString(),{class:['menu-shortcut','unselectable','unresponsible']});
            }
            else this.modify=slabel('');
            if(!(p.style&S_TOGGLE)) this.toggle.setVisible(false);
            if(!(p.style&S_ICON)) this.icon.setVisible(false);
            if(!(p.style&S_LABEL)) this.label.setVisible(false);
            if(!(p.style&S_MODIFIER)) this.modify.setVisible(false);
            this.setSuiID('menuitem').setMainClass('smenu-item').setAction(p.action).setState(p.state)
            .add(this.toggle).add(this.icon).add(this.label).add(sspace()).add(this.modify);
            this.node.onclick=function(e){
                e.stopPropagation();
                if(Cls.parent.suiid==='menubar'&&Cls.submenu){
                    Cls.setActive(!Cls.active);
                    if(Cls.active){
                        Cls.parent.setActive(true);
                        Cls.parent.select(Cls);
                        Cls.showMenu();
                    }
                    else {
                        Cls.parent.setActive(false);
                        Cls.parent.select(null);
                        Cls.hideMenu();
                    }
                }
                else if(Cls.parent.suiid==='menu'&&Cls.submenu){
                    Cls.setActive(!Cls.active);
                    if(Cls.active){
                        Cls.parent.setActive(true);
                        Cls.parent.select(Cls);
                        Cls.showMenu();
                    }
                    else {
                        Cls.parent.setActive(false);
                        Cls.parent.select(null);
                        Cls.hideMenu();
                    }
                }
                if(Cls.parent.suiid==='menu'){
                    if(Cls.parent.mode==='select'){
                        Cls.parent.select(Cls);
                        Cls.setActive(false);
                        Cls.hideMenu();
                    }
                    if(Cls.action) Cls.action(e);
                    Cls.parent.close();
                }
            }
            this.node.onmouseover=function(e){
                if(Cls.parent.suiid==='menubar'&&Cls.parent.active&&Cls.submenu){
                    Cls.parent.select(Cls);Cls.setActive(true).showMenu();
                }
                else if(Cls.parent.suiid==='menu'&&Cls.submenu){
                    Cls.setActive(true).showMenu();
                }
            }
            /*
            this.node.onmouseleave=function(e){
                if(Cls.parent.suiid==='menubar'&&Cls.active){
                    Cls.setActive(false).hideMenu();
                }
                else if(Cls.parent.suiid==='menu'&&Cls.active){
                    Cls.setActive(false).hideMenu();
                }
            }
            */
        }
    },
    setActive:{value:function(a){
        if(a){this.addClass('active');}else{this.removeClass('active');}
        this.active=a;return this;
    }}, 
    setState: {value:function(s){
        this.state=s;
        if(this.style&S_TOGGLE){
            if(this.state) this.toggle.setText(CHECK_CHAR);
            else this.toggle.setText('');
        }
        return this;
    }},
    showMenu: { value: function(){
        if(this.submenu){
            this.submenu.setVisible(true).setX(0).setY(0);
            if(this.expand===BOTTOM||this.expand===TOP){
                if(document.body.clientWidth<this.X()+this.submenu.width()) this.submenu.setX(this.X()+this.width()-this.submenu.width());
                else this.submenu.setX(this.X());
                if(this.expand===BOTTOM) this.submenu.setY(this.Y()+this.height());
                else this.submenu.setY(this.Y()-this.submenu.height());
            }
            else {
                if(document.body.clientHeight<this.Y()+this.submenu.height()) this.submenu.setY(document.body.clientHeight-this.submenu.height());
                else this.submenu.setY(this.Y());
                if(this.expand===RIGHT) this.submenu.setX(this.X()+this.width());
                else this.submenu.setX(this.X()-this.submenu.width());
            }
            CURRENT_MENU=this.submenu;
        }
        return this;
    }},
    hideMenu: {value:function(){if(this.submenu){this.submenu.setVisible(false);} return this;}},
    setLocale:{value:function(l){SUIComponent.prototype.setLocale.apply(this,[l]);if(this.submenu)this.submenu.setLocale(l);}},
    setAction:{value:function(a){this.action=a;return this;}}
});
SMenuItem.prototype.constructor=SMenuItem;
function smenuitem(a){return new SMenuItem(a);}
function SMenu(p){
    p=propOverride(p,{direction:VERTICAL,mode:null,items:[]});
    p.layout=sflow(p.direction);SUIComponent.call(this,'div',p);
}
SMenu.prototype=Object.create(SUIComponent.prototype, {
    initNode:{value:function(p){
        SUIComponent.prototype.initNode.apply(this,[p]);
        const Cls=this;this.mode=p.mode;this.active=false;this.selected=null;
        this.setSuiID('menu').setMainClass('smenu').setItems(p.items);
    }},
    showAt: {
        value: function(u,l){
            this.setVisible(true).setActive(true).setX(0).setY(0);
            if(l===BOTTOM||l===TOP){
                if(document.body.clientWidth<u.X()+this.width()) this.setX(u.X()+u.width()-this.width());
                else this.setX(u.X());
                if(l===BOTTOM) this.setY(u.Y()+u.height());
                else this.setY(u.Y()-this.height());
            }
            else {
                if(document.body.clientHeight<u.Y()+this.height()) this.setY(document.body.clientHeight-this.height());
                else this.setY(u.Y());
                if(l===RIGHT) this.setX(u.X()+u.width());
                else this.setX(u.X()-this.width());
            }
            CURRENT_MENU=this;
    }},
    hide:{value:function(){this.setVisible(false).setActive(false);CURRENT_MENU=null;}},
    close:{ value:function(){
        this.hide();
        var p=this.parent;
        while(true) {
            if(p.suiid==='menuitem') {
                p.setActive(false);p=p.parent;
            }
            else if(p.suiid==='menu') {
                p.setActive(false);p.hide();p=p.parent;
            }
            else if(p.suiid==='menubar') {
                p.setActive(false);p=p.parent;
            }
            else break;
        }
    }},
    setActive:{value:function(a){
        if(a){this.addClass('active');}else{this.removeClass('active');}
        this.active=a;return this;
    }}, 
    select:{value:function(i){
        if(this.selected) this.selected.setState(false);
        this.selected=i;this.selected.setState(true);
    }},
    selectedIndex:{value:function(){return this.selected?this.components.indexOf(this.selected):null;}},
    setItems:{value:function(i){if(i.length){for(var v=0;v<i.length;v++){this.addItem(i[v]);}}}},
    addItem: {
        value: function(i){
            if(i.suiid){
                this.add(i);
                if(this.mode==='select'&&i.state) this.selected=i;
            }
            else {
                if(i.type==='space') this.add(sspace());
                else if(i.type==='bar') this.add(sbar(this.hasClass('vertical')?HORIZONTAL:VERTICAL));
                else {
                    var i_=smenuitem(i);
                    this.add(i_);
                    if(this.mode==='select'&&i.state) this.selected=i_;
                }
            }
        }
    },
    setMode:{value:function(m){this.mode=m;}}
});
SMenu.prototype.constructor=SMenu;
function smenu(a){return new SMenu(a);}
function SMenuBar(p){
    p=propOverride(p,{direction:HORIZONTAL,items:[]});
    this.direction=p.direction;
    p.layout=sflow(this.direction);
    SUIComponent.call(this,'div',p);
}
SMenuBar.prototype=Object.create(SUIComponent.prototype, {
    initNode:{value:function(p){
        SUIComponent.prototype.initNode.apply(this,[p]);
        var Cls=this;this.active=false;this.selected=null;
        this.setSuiID('menubar').addClass('smenu-bar').setItems(p.items);
        this.node.onclick=function(e){
            if(Cls.active&&Cls.selected) Cls.select(null);
        };
    }},
    select:{value:function(i){
        if(this.selected){this.selected.hideMenu().setActive(false);}
        this.selected=i;
        this.setActive(this.selected?true:false);
    }},
    setActive:{value:function(a){this.active=a;if(this.active){this.addClass('active');}else{this.removeClass('active');} return this;}},
    setItems:{value:function(i){if(i.length) for(var v=0;v<i.length;v++){this.addItem(i[v]);}}},
    addItem:{value:function(i){
        if(i.suiid) this.add(i);
        else {
            if(i.type==='space') this.add(sspace());
            else if(i.type==='bar') this.add(sbar(this.direction==VERTICAL?HORIZONTAL:VERTICAL));
            else this.add(smenuitem(i));
        }
        return this;
    }}
});
SMenuBar.prototype.constructor=SMenuBar;
function smenubar(a){return new SMenuBar(a);}
function SToolItem(p){
    p=propOverride(p,{style:S_ICON|S_LABEL,mode:'button',icon:'',label:'item',state:false,available:true,action:null,submenu:null,direction:VERTICAL,expand:BOTTOM});
    this.direction=p.direction;p.layout=sflow(p.direction);
    SUIComponent.call(this,'div',p);
};
SToolItem.prototype=Object.create(SUIComponent.prototype, {
    initNode:{value:function(p){
            SUIComponent.prototype.initNode.apply(this,[p]);
            const Cls=this;this.mode=p.mode;this.style=p.style;this.available=p.available;
            this.icon=null;this.label=null;this.state=false;this.submenu=null;
            if(p.icon) this.icon=parseImage(p.icon);
            else this.icon=slabel('',{class:['unselectable','unresponsible']});
            this.icon.addClass('tool-icon');
            this.label=slabel(p.label,{class: ['tool-label','unselectable','unresponsible']});
            if(!(p.style&S_ICON)) this.icon.setVisible(false);
            if(!(p.style&S_LABEL)) this.label.setVisible(false);
            this.setSuiID('toolitem').setMainClass('stool-item').addClass('align-center')
            .setAvailable(p.available).setAction(p.action)
            .add(this.icon).add(this.label);
            if(p.mode==='toggle') this.setState(p.state);
            if(p.submenu){
                p.submenu.setVisible(false);this.submenu=p.submenu;
                if(p.mode==='select') p.submenu.setMode(S_SELECT);
                this.add(this.submenu);
            }
            this.node.onclick=function(e){
                e.stopPropagation();
                if(Cls.hasClass('unavailable')) return;
                if(p.mode==='toggle') Cls.setState(!Cls.state);
                else if(Cls.submenu){
                    if(Cls.submenu.active) Cls.submenu.hide();
                    else Cls.submenu.showAt(Cls,p.expand);
                }
                if(Cls.action) Cls.action(e);
            };
        }
    },
    setIcon:{value:function(i){
        if(this.icon) {
            if(this.icon.suiid==='text') this.icon.setText(i);
            else if(this.icon.suiid==='icon') this.icon.setIcon(i.substr(i.indexOf(':')+1));
            else if(this.icon.suiid==='image') this.icon.setSrc(i.substr(i.indexOf(':')+1));
        }
    }},
    setState:{value:function(s){
        if(s){this.addClass('active');}else{this.removeClass('active');}
        this.state=s;return this;
    }},
    setAvailable:{value:function(a){
        if(a){this.removeClass('unavailable');}else{this.addClass('unavailable');}
        this.available=a;return this;
    }},
    setAction:{value:function(a){this.action=a;return this;}}
});
SToolItem.prototype.constructor=SToolItem;
function stoolitem(a){return new SToolItem(a);};
function SToolBar(p){
    p=propOverride(p,{autohide:false,direction:HORIZONTAL,items:[]});
    p.layout=sflow(p.direction);this.direction=p.direction;
    SUIComponent.call(this,'div',p);
};
SToolBar.prototype=Object.create(SUIComponent.prototype, {
    initNode:{value:function(p){
            SUIComponent.prototype.initNode.apply(this,[p]);
            const Cls=this;this.autohide=p.autohide;
            this.setSuiID('toolbar').setMainClass('stool-bar');
            if(this.autohide){
                this.addClass('hide');
                this.node.onmouseover=function(e){Cls.show();}
                this.node.onmouseleave=function(e){Cls.hide();}
            }
            for(var i=0;i<p.items.length;i++){ this.addItem(p.items[i]);}
        }},
    addItem:{value:function(i){
        if(i.suiid) this.add(i);
        else{
            if(i.type==='space') this.add(sspace());
            else if(i.type==='bar') this.add(sbar(this.direction==HORIZONTAL?VERTICAL:HORIZONTAL));
            else this.add(stbitem(i));
        }
        return this;
    }},
    show:{value:function(){this.removeClass('hide');}},
    hide:{value: function(){this.addClass('hide');}}
});
SToolBar.prototype.constructor=SToolBar;
function stoolbar(a){return new SToolBar(a);};
function SLabel(t,p){p=propOverride(p,{});SUIComponent.call(this,'span',p);this.setText(t);};
SLabel.prototype=Object.create(SUIComponent.prototype, {
    initNode:{value:function(p){
        SUIComponent.prototype.initNode.apply(this,[p]);
        this.txt='';this.setSuiID('label').setMainClass('slabel');
    }},
    text:{value:function(){return this.node.innerText;}},
    setText:{value:function(t){this.txt=t;
        if(this.isLocalized()&&localizable(this.locale,t)){this.node.innerHTML=LOCAL_LABEL[t][this.locale];}
        else{this.node.innerHTML=t;} return this;
    }},
    setLocale:{value:function(l){SUIComponent.prototype.setLocale.apply(this,[l]); this.setText(this.txt); return this;}}
});
SLabel.prototype.constructor=SLabel;
function slabel(a,b){return new SLabel(a,b);};
function SLinkLabel(t,u,p){p=propOverride(p,{newtab:false});SUIComponent.call(this,'a',p);this.setText(t).setURL(u);};
SLinkLabel.prototype=Object.create(SUIComponent.prototype, {
    initNode:{value:function(p){
        SUIComponent.prototype.initNode.apply(this,[p]);
        this.txt='';this.setSuiID('link').setMainClass('slink');
        if(p.newtab) this.setAttribute({target:'_blank',rel:'noopener noreferrer'});
    }},
    text:{value:function(){return this.node.innerText;}},
    setText:{value:function(t){this.txt=t;
        if(this.isLocalized()&&localizable(this.locale,t)){this.node.innerText=LOCAL_LABEL[t][this.locale];}
        else{this.node.innerText=t;}return this;
    }},
    setURL:{value:function(u){if(u){this.node.href=u;} else{this.node.href='javascript:void(0)'} return this;}},
    setLocale:{value:function(l){
        SUIComponent.prototype.setLocale.apply(this,[l]);
        this.setText(this.txt); return this;
    }}
});
SLinkLabel.prototype.constructor=SLinkLabel;
function slink(a,b,c){return new SLinkLabel(a,b,c);};
function SIcon(i,p){p=propOverride(p,{});SUIComponent.call(this,'i',p);this.node.innerText=i;};
SIcon.prototype=Object.create(SUIComponent.prototype, {
    initNode:{value:function(p){
        SUIComponent.prototype.initNode.apply(this,[p]);
        this.setSuiID('icon').setMainClass('sicon');
    }},
    setIcon:{value:function(i){this.node.innerText=i;}}
});
SIcon.prototype.constructor=SIcon;
function sicon(a,b){return new SIcon(a,b);};
function SForm(p){p=propOverride(p,{key:'api',method:'post'});SUIComponent.call(this,'form',p);};
SForm.prototype=Object.create(SUIComponent.prototype,{
    initNode:{value:function(p){
        SUIComponent.prototype.initNode.apply(this,[p]);
        this.setSuiID('form').setMainClass('sform').setAttribute({action:p.key,method:p.method});
    }},
    submit:{value:function(){this.node.submit();}},
    setKey:{value:function(k){this.node.action=k;return this;}},
    setMethod:{value:function(m){this.node.method=m;return this;}},
});
SForm.prototype.constructor=SForm;
function sform(a){return new SForm(a);};
function SInput(t,p){p=propOverride(p,{value:null});SUIComponent.call(this,'input',p);this.node.type=t;};
SInput.prototype=Object.create(SUIComponent.prototype,{
    initNode:{value:function(p){
        SUIComponent.prototype.initNode.apply(this,[p]);
        this.setSuiID('input').setMainClass('sinput').setValue(p.value);
    }},
    value:{value:function(){return this.node.value;}},
    setValue:{value:function(v){if(v) this.node.value=v;return this;}},
    setType:{value:function(t){this.node.type=t;return this;}}
});
SInput.prototype.constructor=SInput;
function sinput(a,b){return new SInput(a,b);};
function STextField(p){
    p=propOverride(p,{type:'text',label:'',value:'',placeholder:'',selectable:true,editable:true,required:false});
    SUIComponent.call(this,'label',p);
};
STextField.prototype=Object.create(SUIComponent.prototype, {
    initNode:{value:function(p){
        SUIComponent.prototype.initNode.apply(this,[p]);
        this.label=slabel(p.label,{class:['field-label']});this.input=sinput(p.type,p);
        if(p.type==='number'){this.setSuiID('numericfield').setMainClass('snumber-field');}
        else if(p.type==='password'){this.setSuiID('passfield').setMainClass('spass-field');}
        else if(p.type==='text'){this.setSuiID('textfield').setMainClass('stext-field');}
        this.add(this.label).add(this.input).setValue(p.value).setPlaceholder(p.placeholder).setSelectable(p.selectable).setEditable(p.editable).setRequired(p.required);
    }},
    placeholder:{value:function(){return this.input.node.placeholder;}},
    setValue:{value:function(v){if(v) this.input.node.value=v; return this;}},
    setPlaceholder:{value:function(t){if(t) this.input.node.placeholder=t; return this;}},
    isSelectable:{value:function(){return !this.hasClass('unselectable');}},
    setSelectable:{value:function(s){if(s) this.removeClass('unselectable'); else{this.addClass('unselectable');} return this;}},
    isEditable:{value:function(){return !this.input.node.readOnly;}},
    setEditable:{value:function(e){this.input.node.readOnly=!e; return this;}},
    isReqired:{value:function(){return this.input.node.required;}},
    setRequired:{value:function(r){this.input.node.required=r; return this;}}
});
STextField.prototype.constructor=STextField;
function stextfield(a){return new STextField(a);};
function STextArea(p){
    p=propOverride(p,{label:'',value:'',placeholder:'',selectable:true,editable:true,required:false});
    SUIComponent.call(this,'label',p);
};
STextArea.prototype=Object.create(SUIComponent.prototype, {
    initNode:{
        value:function(p){
            SUIComponent.prototype.initNode.apply(this,[{}]);
            this.label=slabel(p.label, {class:['area-label','valign-top']});this.area=sinput('textarea',p);
            this.setSuiID('textarea').setMainClass('stext-area').add(this.label).add(this.area).setValue(p.text).setPlaceholder(p.placeholder).setSelectable(p.selectable).setEditable(p.editable).setRequired(p.required);
        }
    },
    value:{value:function(){return this.area.node.innerText;}},
    setValue:{value:function(txt){if(txt!=undefined) this.area.node.innerText=txt; return this;}},
    placeholder:{value:function(){return this.node.placeholder;}},
    setPlaceholder:{value:function(txt){if(txt!=undefined) this.node.placeholder=txt; return this;}},
    isSelectable:{value:function(){return !this.hasClass('unselectable');}},
    setSelectable:{value:function(sel){
        if(sel){this.removeClass('unselectable');}
        else{this.addClass('unselectable');} return this;
    }},
    isEditable:{value:function(){return !this.node.readOnly;}},
    setEditable:{value:function(edit){this.node.readOnly=!edit; return this;}},
    isReqired:{value:function(){return this.node.required;}},
    setRequired:{value:function(req){this.node.required=req; return this;}}
});
STextArea.prototype.constructor=STextArea;
function stextarea(a){return new STextArea(a);};
function STextView(p){
    p=propOverride(p,{value:'',selectable:true,editable:false});
    SUIComponent.call(this,'p',p);
};
STextView.prototype=Object.create(SUIComponent.prototype, {
    initNode:{value:function(p){
        SUIComponent.prototype.initNode.apply(this,[p]);
        this.setSuiID('textview').setMainClass('stext-view').setValue(p.value).setSelectable(p.selectable).setEditable(p.editable);
    }},
    value:{value:function(){return this.node.innerHTML;}},
    setValue:{value:function(t){this.node.innerHTML=t; return this;}},
    isSelectable:{value:function(){return !this.hasClass('unselectable');}},
    setSelectable:{value:function(s){
        if(s){this.removeClass('unselectable');}
        else{this.addClass('unselectable');} return this;
    }},
    isEditable:{value:function(){return this.node.contentEditable;}},
    setEditable:{value:function(e){this.node.contentEditable=e; return this;}}
});
STextView.prototype.constructor=STextView;
function stextview(a){return new STextView(a);};
function SMathText(m,p){
    p=propOverride(p,{});
    SUIComponent.call(this,'code',p);
    this.node.innerHTML=codedString('\\('+m+'\\)');
};
SMathText.prototype=Object.create(SUIComponent.prototype, {
    initNode:{value:function(p){
        SUIComponent.prototype.initNode.apply(this,[p]);
        this.setSuiID('math').setMainClass('smath');
    }},
    value:{value:function(){return this.node.innerHTML;}},
    setValue:{value:function(t){this.node.innerHTML='\\('+t+'\\)'; return this;}},
});
SMathText.prototype.constructor=SMathText;
function smath(a,b){return new SMathText(a,b);};
function SButton(p){
    p=propOverride(p,{label:'',image:null,state:false,available:true,action:null,mode:null,style:S_LABEL});
    if(p.mode) SUIComponent.call(this,'div',p);
    else SUIComponent.call(this,'button',p);
};
SButton.prototype=Object.create(SUIComponent.prototype,{
    initNode:{value:function(p){
        SUIComponent.prototype.initNode.apply(this,[p]);
        const Cls=this;this.state=null;this.available=null;this.action=null;
        if(p.mode==='panel') {
            this.content=p.content?p.content:spanel();
            this.content.setMainClass('button-panel');
            this.setMainClass('scustom-button').setDisplay('inline-block').add(this.content);
        }
        else if(p.mode==='push') this.setMainClass('spush-button').setDisplay('inline-block');
        else if(p.mode==='toggle') {
            if(p.image) this.image=[parseImage(p.image[0]),parseImage(p.icon[1])];
            else this.image=[sicon('toggle_off'),sicon('toggle_on')];
            this.image[0].addClass('button-image');
            this.image[1].addClass('button-image');
            this.setMainClass('stoggle-button').setDisplay('inline-block').setState(p.state);
        }
        else{
            this.label=slabel(p.label,{class:['button-label','unselectable','unresponsible']});
            this.setMainClass('sbutton').add(this.label);
        }
        this.node.onclick=function(e){
            if(Cls.available){Cls.setState(!Cls.state);if(Cls.action){Cls.action(e);}}
        };
        this.setSuiID('button').setState(p.state).setAvailable(p.available).setAction(p.action);
    }},
    setLabel:{value:function(t){if(this.label){this.label.setText(t);} return this;}},
    setLabels:{value:function(t){this.labels=t; return this;}},
    setState:{value:function(s){
        this.state=s;
        if(this.image&&this.image.length==2) {this.clear();this.add(this.image[s?1:0]);}
        if(this.state){this.addClass('active');}else{this.removeClass('active');} return this;
    }},
    setAvailable:{value:function(a){
        this.available=a;if(a){this.removeClass('unavailable');} else{this.addClass('unavailable');} return this;
    }},
    setAction:{value:function(a){this.action=a; return this;}}
});
SButton.prototype.constructor=SButton;
function sbutton(a){return new SButton(a);};
function SRadioButton(p){p=propOverride(p,{label:'',state:false,available:true,action:null});SUIComponent.call(this,'label',p);};
SRadioButton.prototype=Object.create(SUIComponent.prototype, {
    initNode:{value:function(p){
        SUIComponent.prototype.initNode.apply(this,[{}]);
        this.label=slabel(p.label,{class:['sradio-label']});this.input=sinput('radio',p);
        this.setSuiID('radio').setMainClass('sradio').add(this.input).add(this.label).setState(p.state).setAvailable(p.available).setAction(p.action);
    }},
    state:{value:function(){return this.input.node.checked;}},
    setState:{value:function(s){this.input.node.checked=s;return this;}},
    isAvailable:{value:function(){return !this.input.isAvailable();}},
    setAvailable:{value:function(a){this.input.setAvailable(a);return this;}},
    setAction:{value:function(a){if(a) this.input.node.onchange=a;return this;}}
});
SRadioButton.prototype.constructor=SRadioButton;
function sradio(a){return new SRadioButton(a);};
function SCheckBox(p){p=propOverride(p,{label:'',state:false,available:true,action:null});SUIComponent.call(this,'label',p);};
SCheckBox.prototype=Object.create(SUIComponent.prototype, {
    initNode:{value:function(p){
        SUIComponent.prototype.initNode.apply(this, [{}]);
        this.label=slabel(p.label,{class:['scheck-label']});this.input=sinput('checkbox',p);
        this.setSuiID('checkbox').setMainClass('scheckbox').add(this.input).add(this.label).setState(p.state).setAvailable(p.available).setAction(p.action);
    }},
    state:{value:function(){return this.input.node.checked;}},
    setState:{value:function(s){this.input.node.checked=s;return this;}},
    isAvailable:{value:function(){return !this.input.isAvailable();}},
    setAvailable:{value:function(a){this.input.setAvailable(a);return this;}},
    setAction:{value:function(a){if(a) this.input.node.onchange=a;return this;}}
});
SCheckBox.prototype.constructor=SCheckBox;
function scheck(a){return new SCheckBox(a);};
function SSelectItem(p){
    p=propOverride(p,{group:false,value:'item',label:'item'});
    if(p.group) SUIComponent.call(this,'optgroup',p);
    else SUIComponent.call(this,'option',p);
};
SSelectItem.prototype=Object.create(SUIComponent.prototype, {
    initNode:{value:function(p){
        SUIComponent.prototype.initNode.apply(this,[p]);
        this.label=slabel(p.label);
        if(p.group) this.setSuiID('selectgroup').setMainClass('sselect-group').add(this.label);
        else this.setSuiID('selectitem').setMainClass('sselect-item').add(this.label).setValue(p.value);
    }},
    setLabel:{value:function(l){this.label.setText(l);return this;}},
    value:{value:function(v){return this.node.value;}},
    setValue:{value:function(v){this.node.value=v;return this;}},
    setSelected:{value:function(s){this.node.selected=s;return this;}}
});
SSelectItem.prototype.constructor=SSelectItem;
function sselitem(a){return new SSelectItem(a);};
function SSelector(p){p=propOverride(p,{multi:false,selected:1,items:[],action:null});SUIComponent.call(this,'select',p);};
SSelector.prototype=Object.create(SUIComponent.prototype, {
    initNode:{value:function(p){
        SUIComponent.prototype.initNode.apply(this,[p]);
        const Cls=this;this.multi=p.multi;
        this.setSuiID('selector').setMainClass(['sselector']).setMultiSelectable(p.multi).setOptions(p.items).selectAt(p.selected).setAction(p.action);
    }},
    setMultiSelectable:{value:function(m){this.node.multiple=m; return this;}},
    setOptions:{value:function(o){for(var i=0;i<o.length;i++){this.addOption(o[i]);} return this;}},
    select:{value:function(o){this.components[o].node.selected=true; return this;}},
    selectValue:{value:function(v){for(var o=0;o<this.components.length;o++){if(this.components[o].node.value===v){this.components[o].node.selected=true;break;}}  return this;}},
    selectAt:{value:function(i){if(i<=this.components.length){this.node.selectedIndex=i-1;} return this;}},
    addOption:{value:function(o){
        if(o.suiid==='selectitem'||o.suiid==='selectgroup'){this.add(o);}
        else{this.add(sselitem(o));} return this;
    }},
    setAction:{value:function(a){if(a) this.node.onchange=function(e){a(e);}; return this;}},
    selectedIndex:{value:function(){return this.node.selectedIndex;}},
    selectedItem:{value:function(){return this.node.selectedOptions[0];}},
    selectedItems:{value:function(){return this.node.selectedOptions;}}
});
SSelector.prototype.constructor=SSelector;
function sselect(a){return new SSelector(a);};
function SSlider(p){p=propOverride(p,{label:'',min:0,max:100,value:0,action:null});SUIComponent.call(this,'label',p);};
SSlider.prototype=Object.create(SUIComponent.prototype, {
    initNode:{value:function(p){
        SUIComponent.prototype.initNode.apply(this,[{}]);
        this.label=slabel(p.label,{class:['slider-label']});this.input=sinput('range',p);
        this.setSuiID('slider').setMainClass('sslider').add(this.input).add(this.label).setMin(p.min).setMax(p.max).setValue(p.value).setAction(p.action);
    }},
    minValue:{value:function(){return this.input.node.min;}},
    setMin:{value:function(m){this.input.node.min=m; return this;}},
    maxValue:{value:function(){return this.input.node.max;}},
    setMax:{value:function(m){this.input.node.max=m; return this;}},
    current:{value:function(){return this.input.node.value;}},
    setValue:{value:function(v){
        this.input.node.value=v;this.input.node.caption=v;return this;
    }},
    setAction:{value:function(a){if(a) this.input.node.onchange=function(e){a(e);}; return this;}}
});
SSlider.prototype.constructor=SSlider;
function sslider(a){return new SSlider(a);};
function SProgress(p){
    p=propOverride(p,{min:0,max:100,value:0,direction:HORIZONTAL,repaint:null});
    this.direction=p.direction;SUIComponent.call(this,'div',p);
};
SProgress.prototype=Object.create(SUIComponent.prototype, {
    initNode: {value:function(p){
        SUIComponent.prototype.initNode.apply(this,[p]);
        this.setSuiID('progress').setMainClass('sprogress');
        this.min=p.min;this.max=p.max;this.value=p.value;this.spacer=sspace();
        this.indicator=sview({width:'100%',height:'100%',class:['prog-indicator']});
        this.repaint=p.repaint;
        switch(this.direction){
            case HORIZONTAL:
                this.setLayout(sflow(HORIZONTAL)).addClass('horizontal').add(this.indicator).add(this.spacer);break;
            case VERTICAL:
                this.setLayout(sflow(VERTICAL)).addClass('vertical').add(this.spacer).add(this.indicator);break;
        }
    }},
    getValue:{value:function(){return current;}},
    setValue:{value: function(v){if(v<=this.max&&this.min<=v){this.value=v;this.update();} return this;}},
    setAction:{value:function(a){if(a) this.input.node.onchange=function(e){a(e);}; return this;}},
    update:{value:function(){
        switch(this.direction){
            case HORIZONTAL:
                {
                    if(this.repaint) this.repaint(this.value);
                    else this.spacer.setWidth((1.0-(this.value-this.min)/(this.max-this.min))*100+'%');
                }
                break;
            case VERTICAL:
                {
                    if(this.repaint) this.repaint(this.value);
                    else this.spacer.setHeight((1.0-(this.value-this.min)/(this.max-this.min))*100+'%');
                }
                break;
            default:
                {
                    if(this.repaint){this.repaint(this.value);}
                }
                break;
        }
    }}
});
SProgress.prototype.constructor=SProgress;
function sprogress(a){return new SProgress(a);};
function STimer(d,f,r){this.thread=null;this.active=false;this.rep=r;this.duration=d;this.action=f;};
STimer.prototype={
    start:function(){
        const Cls=this;
        if(1<this.rep){this.thread=setInterval(function(){Cls.action(Cls.thread, Cls.rep);}, Cls.duration);}
        else{this.thread=setTimeout(function(){Cls.action(Cls.thread);}, Cls.duration);}
    },
    stop:function(){if(1<this.rep){clearInterval(this.thread);}else{clearTimeout(this.thread);}}
};
function stimer(a,b,c){return new STimer(a,b,c);};

////
function SCalender(){

};
SCalender.prototype=Object.create(SUIComponent.prototype, {
    initNode:{
        value:function(p){
            SUIComponent.prototype.initNode.apply(this,[{}]);
        }
    },
});
SCalender.prototype.constructor=SCalender;

function SDatePicker(p){
    if(p == null) p={};
    p.sclass='datepicker';
    if(p.class == undefined) p.class=['sdatepicker'];
    else p.class.push('sdatepicker');
    SUIComponent.call(this, 'input', p);
};
SDatePicker.prototype=Object.create(SUIComponent.prototype, {
    initNode:{
        value:function(p){
            SUIComponent.prototype.initNode.apply(this, [ p ]);
            this.node.type='date';
            this.setFormat(p.format==undefined?'yyyy/mm/dd':p.format);
            this.setValue(p.value==undefined?(new Date()).toDateString():p.value);
        }
    },
    setFormat:{value:function(form){this.node.format=form;}},
    setValue:{value:function(val){this.node.value=val;}}
});
SDatePicker.prototype.constructor=SDatePicker;
////

function SFileLoader(p){
    p=propOverride(p,{id:'sfl',label:'file-select',filter:null,action:null});
    this.accessible=true;this.files=null;
    SUIComponent.call(this,'div',p);
};
SFileLoader.prototype=Object.create(SUIComponent.prototype, {
    initNode:{value:function(p){
        SUIComponent.prototype.initNode.apply(this,[{}]);
        const Cls=this;
        this.input=sinput('file',{id:p.id,visible:false});
        this.input.node.onchange=function(e){
            Cls.files=this.files;if(p.action){p.action(e);}
        };
        this.label=suic('label');
        this.label.setMainClass('fl-label').add(slabel(p.label,{class:['unselectable','unresponsible']}));
        this.label.node.htmlFor=p.id;
        this.setSuiID('fileloader').setMainClass('sfileloader').setFilter(p.filter).add(this.input).add(this.label);
    }},
    setFilter: {value:function(f){if(f){this.input.node.accept=f;}return this;}}
});
SFileLoader.prototype.constructor=SFileLoader;
function sfileloader(a){return new SFileLoader(a);};