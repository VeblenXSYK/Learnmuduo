/**
 * 更新系统时间
 */
var systemTime;
function showSystemTime(){
	if(systemTime) clearTimeout(systemTime);
	var date = new Date();
	$("#refreshTime>span:eq(1)").html(date.pattern("yyyy-MM-dd  HH:mm:ss EEE"));
	systemTime = setTimeout("showSystemTime();",1000);
}

/**
 * 菜单点击事件
 * @param url
 * @param obj
 */
function menuClick(url,obj){
	$("li[role='presentation'].active").removeClass("active");
	$(obj).addClass("active");
	$.ajax({
		url: url,
		type: 'GET',
		cache:false,
		complete: function(response) {
			if(response.status == 200) {
				$(".noData").hide();
				$("#contentFrame").attr("src",url);
			} else {
				$(".noData").show();
			}
		}
	});
}

function menuToggleClick(){
	var left = $("div.containers").css("left");
	if(left == "60px"){
		$("div.containers").css("left","150px");
	}else{
		$("div.containers").css("left","60px");
	}
}

function menuInit(){
	var event = 'click';
	$.each($("div.menu>ul>li"),function(i,obj){
		var $this = this;
		var url = $(this).data("url");
		$(this).on(event,function(){
			menuClick(url, $this);
		});
	});
}

$(function(){
	showSystemTime();
	menuInit();
	menuClick('Control.html',$("div.menu>ul>li:eq(0)"));
	$("#menuToggle").deviceEvent("click",function(){menuToggleClick();});
});
