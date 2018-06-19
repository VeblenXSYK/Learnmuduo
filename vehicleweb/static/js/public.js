/** 
 * 对Date的扩展，将 Date 转化为指定格式的String 
 * 月(M)、日(d)、12小时(h)、24小时(H)、分(m)、秒(s)、周(E)、季度(q)
   可以用 1-2 个占位符 * 年(y)可以用 1-4 个占位符，毫秒(S)只能用 1 个占位符(是 1-3 位的数字) 
 * eg: 
 * (new Date()).pattern("yyyy-MM-dd hh:mm:ss.S")==> 2006-07-02 08:09:04.423      
 * (new Date()).pattern("yyyy-MM-dd E HH:mm:ss") ==> 2009-03-10 二 20:09:04      
 * (new Date()).pattern("yyyy-MM-dd EE hh:mm:ss") ==> 2009-03-10 周二 08:09:04      
 * (new Date()).pattern("yyyy-MM-dd EEE hh:mm:ss") ==> 2009-03-10 星期二 08:09:04      
 * (new Date()).pattern("yyyy-M-d h:m:s.S") ==> 2006-7-2 8:9:4.18      
 */        
Date.prototype.pattern=function(fmt) {         
	var o = {         
			"M+" : this.getMonth()+1, //月份         
			"d+" : this.getDate(), //日         
			"h+" : this.getHours()%12 == 0 ? 12 : this.getHours()%12, //小时         
			"H+" : this.getHours(), //小时         
			"m+" : this.getMinutes(), //分         
			"s+" : this.getSeconds(), //秒         
			"q+" : Math.floor((this.getMonth()+3)/3), //季度         
			"S" : this.getMilliseconds() //毫秒         
	};         
	var week = {         
			"0" : "\u65e5",         
			"1" : "\u4e00",         
			"2" : "\u4e8c",         
			"3" : "\u4e09",         
			"4" : "\u56db",         
			"5" : "\u4e94",         
			"6" : "\u516d"        
	};         
	if(/(y+)/.test(fmt)){         
		fmt=fmt.replace(RegExp.$1, (this.getFullYear()+"").substr(4 - RegExp.$1.length));         
	}         
	if(/(E+)/.test(fmt)){         
		fmt=fmt.replace(RegExp.$1, ((RegExp.$1.length>1) ? (RegExp.$1.length>2 ? "\u661f\u671f" : "\u5468") : "")+week[this.getDay()+""]);         
	}         
	for(var k in o){         
		if(new RegExp("("+ k +")").test(fmt)){         
			fmt = fmt.replace(RegExp.$1, (RegExp.$1.length==1) ? (o[k]) : (("00"+ o[k]).substr((""+ o[k]).length)));         
		}         
	}         
	return fmt;         
};

$.fn.offDeviceEvent = function(type,func){
	type = type.toLowerCase();
	if(func){
		this.off(type,func);
	}else{
		this.unbind(type);
	}
	return this;
}

 $.fn.deviceEvent = function(type,func){
	$(this).unbind(type).bind(type,func);
	return this;
};

var error = {
		"400":"当前访问操作无法访问服务器",
		"500":"服务器内部错误",
		"500.11":"服务器错误：Web 服务器上的应用程序正在关闭",
		"500.12":"服务器错误：Web 服务器上的应用程序正在重新启动",
		"500.13":"服务器错误：Web 服务器太忙",
		"500.14":"服务器错误：服务器上的无效应用程序配置",
		"500.15":"服务器错误：不允许直接请求 GLOBAL.ASA",
		"500.16":"服务器错误：UNC 授权凭据不正确",
		"500.17":"服务器错误：URL 授权存储无法找到",
		"500.18":"服务器错误：URL 授权存储无法打开",
		"500.19":"服务器错误：该文件的数据在配置数据库中配置不正确",
		"500.20":"服务器错误：URL 授权域无法找到"
}

function diyAlert(content){
	var prompt = null;
	if($("#prompt").length==0){
		prompt = $("<div class='modal fade' id='prompt' tabindex='-1' role='dialog'"+
				"aria-labelledby='promptLabel'>"+
				"<div class='modal-dialog' role='document'>"+
				"<div class='modal-content'>"+
				"<div class='modal-header'>"+
				"<button type='button' class='close' data-dismiss='modal'"+
				"aria-label='Close'>"+
				"<span aria-hidden='true'>&times;</span>"+
				"</button>"+
				"<h4 class='modal-title' id='promptLabel'>" +
				"<span class='glyphicon glyphicon-alert'></span>&nbsp;&nbsp;<span class=’glyphicon-class'>提示...</span>" +
				"</h4>"+
				"</div>"+
				"<div class='modal-body'>"+

				"</div>"+
				"<div class='modal-footer'>"+
				"<button type='button' class='btn btn-dan' data-dismiss='modal'>" +
				"<span class='glyphicon glyphicon-remove'></span><span class=’glyphicon-class'>关闭</span>" +
				"</button>"+
				"</div>"+
				"</div>"+
				"</div>"+
		"</div>");
		$("body").append(prompt);
	}else{
		prompt = $("#prompt");
	}
	$("#prompt").on("hidden.bs.modal",function(){
		diyAlertHide();
	});
	$("#prompt .modal-body").html(content);
	$("#prompt").modal("show");
}

function diyAlertHide(){
	$("#prompt").modal("hide");
}

/**
 * 获取bootstrap model
 * @param id
 * @returns
 */
function getBootStrapModel(id){
	return  $("<div class='modal fade' id='"+id+"' tabindex='-1' role='dialog'"+
			"aria-labelledby='promptLabel'>"+
			"<div class='modal-dialog' role='document'>"+
			"<div class='modal-content'>"+
			"<div class='modal-header'>"+
			"<button type='button' class='close' data-dismiss='modal'"+
			"aria-label='Close'>"+
			"<span aria-hidden='true'>&times;</span>"+
			"</button>"+
			"<h4 class='modal-title' id='promptLabel'>" +
			"<span class='glyphicon glyphicon-alert'></span>&nbsp;&nbsp;<span class='glyphicon-class'>提示...</span>" +
			"</h4>"+
			"</div>"+
			"<div class='modal-body'>"+

			"</div>"+
			"<div class='modal-footer'>"+
			"<button type='button' class='btn btn-dan' data-dismiss='modal'>" +
			"<span class='glyphicon glyphicon-remove'></span><span class=’glyphicon-class'>关闭</span>" +
			"</button>"+
			"<button type='button' class='btn btn-success'>" +
			"<span class='glyphicon glyphicon-ok'></span><span class=’glyphicon-class'>确定</span>" +
			"</button>"+
			"</div>"+
			"</div>"+
			"</div>"+
	"</div>");
}

/**
 * 移除加载信息
 */
function loadRemove(){
	$("#model").remove();
}
