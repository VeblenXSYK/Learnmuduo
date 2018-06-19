// 全局变量
var currentAxelNum = 2;
var currentweightType = 0;
var tableInfos = [];

// 弹出框
function toStartDialog(gaugeweight){
	var toStartDialog = getBootStrapModel("toStartDialog");
	toStartDialog.find("div.modal-header h4>span:eq(1)").html("静态标定");
	toStartDialog.on("hidden.bs.modal",function(){
		toStartDialog.remove();
	});
	var form = $("<form class='form-horizontal'></form>");
	var realinfo = $("<div class='form-group'>" +
		"<label for='real_weight' class='col-sm-2 control-label'>实际重量</label>" +
		"<div class='col-sm-10'>" +
		"<input class='form-control' id='real_weight' type='text' value='" + 0 + "' />" +
		"</div>"+
		"</div>");
	var gaugeinfo = $("<div class='form-group'>" +
		"<label for='gauge_weight' class='col-sm-2 control-label'>仪表重量</label>" +
		"<div class='col-sm-10'>" +
		"<input class='form-control' id='gauge_weight' type='text' value='" + gaugeweight + "' />" +
		"</div>"+
		"</div>");
	form.append(realinfo).append(gaugeinfo)
	toStartDialog.find(".modal-body").html(form);
	toStartDialog.find(".modal-footer button:eq(1)").deviceEvent("click",function(){
		$.ajax({
			url : "/Meter/StopStatic",
			type:"GET",
			dataType:"json",
			data:{
				weightype: currentweightType,
				realweight:$("#toStartDialog #real_weight").val(),
				weight:$("#toStartDialog #gauge_weight").val()
			},
			error:function(XMLHttpRequest, textStatus, errorThrown){
				if(error[XMLHttpRequest.status]){
					diyAlert("网络错误,无法连接服务器,错误编码"+XMLHttpRequest.status+":"+error[XMLHttpRequest.status]);
				}else{
					diyAlert("网络错误,无法连接服务器,错误编码"+XMLHttpRequest.status);
				}
			},
			success:function(obj){
				if(obj.result==0){
					toStartDialog.modal("hide");
					
					$("#platAweight>span:eq(1)").html('0');
					$("#platBweight>span:eq(1)").html('0');
				}
			},
			complete:function(XMLHttpRequest){
				loadRemove();
			}
		});
	});
	toStartDialog.modal("show");
}

//开始标定
function startStatic(wtype) {
	$.ajax({
		url: "/Meter/StartStatic", 
		type: "GET",
		dataType:"json",
		timeout: 1000,
		data:{
			weightype:wtype
		},
		error:function(XMLHttpRequest, textStatus, errorThrown)
		{
			if(error[XMLHttpRequest.status]){
				diyAlert("网络错误,错误编码"+XMLHttpRequest.status+":"+error[XMLHttpRequest.status]);
			}else{
				diyAlert("网络错误,错误编码"+XMLHttpRequest.status);
			}
		},
		success:function(obj)
		{
			var type = "A";
			if (wtype == 1) {
				type = "B";
			} 
			if(obj.result && obj.result == 0){
				diyAlert("开始静态标定"+"成功");
				currentweightType = wtype;
			}else{
				diyAlert("开始静态标定"+"失败，原因："+obj.errmsg);
			}
		},
	});
}

//清零
function setZero(wtype) {
	$.ajax({
		url: "/Meter/SetZero", 
		type: "GET",
		dataType:"json",
		timeout: 1000,
		data: {
			weightype: wtype
		},
		error:function(XMLHttpRequest, textStatus, errorThrown)
		{
			if(error[XMLHttpRequest.status]){
				diyAlert("网络错误,错误编码"+XMLHttpRequest.status+":"+error[XMLHttpRequest.status]);
			}else{
				diyAlert("网络错误,错误编码"+XMLHttpRequest.status);
			}
		},
		success:function(obj)
		{
			var type = "A";
			if (currentweightType == 1) {
				type = "B";
			} 
			if(obj.result && obj.result == 0){
				diyAlert("称台"+"置零成功");
			}else{
				diyAlert("称台"+"置零失败，原因："+obj.errmsg);
			}
		},
	});
}

//设置轴详细信息
function setAxleinfo(axleinfo, axlecount) {
	
	for (var i = 0; i < 6; i++) 
	{				
		var id_num = "#table_axleinfo tr:nth-child(" + (i+1) + ") td:nth-child(1)"
		var id_weight = "#table_axleinfo tr:nth-child(" + (i+1) + ") td:nth-child(2)"
		
		if (i < axlecount)
		{
			$(id_num).html(i+1);
			$(id_weight).html(axleinfo[i]["aw"]);
		}
		else
		{
			$(id_num).html("");
			$(id_weight).html("");
		}	
	}
}

//查询历史信息
function getHistory(obj) {
	
	var gska = obj["gska"];
	$("#gskavalue>span:eq(1)").html(gska);
	
	var allWeight = obj["weight"];
	$("#allweight>span:eq(1)").html(allWeight);
	
	var axleinfo = obj["axleinfo"];
	var axlecount = axleinfo.length;
	$("#axlecount>span:eq(1)").html(axlecount);
	setAxleinfo(axleinfo, axlecount);
	
	var lightstat = obj["light"];
	var seninfos = obj["seninfo"];
	updateDevState(lightstat,seninfos);
}

//表格点击事件
function tableClick(value) {
	var clickIndex = Number(value.querySelector('td').textContent) - 1;
	var obj = tableInfos[clickIndex];
	getHistory(obj);
}

//更新设备状态
function updateDevState(lightstat,seninfos) 
{
	// 确定传感器的个数
	var senNum = seninfos.length;

	$('#sen-table-body tr').remove();

	var b = senNum % 2
	var devidNum = 0;
	if (b == 0) {
		devidNum = senNum;
	} else if (b == 1) {
		devidNum = senNum + 1;
	}

	var howMuchRow = devidNum / 2;

	var trHeight = 275 / howMuchRow;

	for (var i=0;i < howMuchRow;i++) {
		var tr = $('<tr valign="middle" class="success" style="height: '+ trHeight+'px"></tr>');
		var k = 2 * i;
		var sN1 = devidNum - (k+1);
		var sN2 = devidNum - (k);
		var sId1 = "button_sensor" + sN1;
		var sId2 = "button_sensor" + sN2;


		var td1 = $('<td><button id=' + sId1 + ' type="button" class="btn btn-danger btn-circle">'+ sN1+'</button></td>');
		var td2 = $('<td align="right"><button id=' + sId2 + ' type="button" class="btn btn-danger btn-circle">'+ sN2 +'</button></td>');
		if (b == 1 && i == 0) {
			td2 = $('<td align="right"></td>')
		}
		tr.append(td1).append(td2);
		$('#sen-table-body').append(tr);
	}

	//更新传感器状态
	for (var i = 0; i < senNum; i++)
	{
		var info = seninfos[i]["s"];
		var id = "#button_sensor" + (i+1);
		
		if (info == "0")
		{
			$(id).removeClass("btn-danger");
			$(id).addClass("btn-success");
		}
		else
		{
			$(id).removeClass("btn-success");
			$(id).addClass("btn-danger");
		}
	}

	// //更新光幕状态
	// if (lightstat == "1")
		// $("#pic_light").attr("src", "images/light_problem.png");
	// else
		// $("#pic_light").attr("src", "images/light_normal.png");
}

//更新系统信息
var reloadTime = 2000;
var systemStateTimeout;
var tablecolor = 0;
function systemState(){
	if(systemStateTimeout) clearTimeout(systemStateTimeout);
	$.ajax({
		type:"GET",
		url:"/CurrentStatus",
		dataType:"json",
		error:function(XMLHttpRequest, textStatus, errorThrown){
			if(error[XMLHttpRequest.status]){
				diyAlert("网络错误,错误编码"+XMLHttpRequest.status+":"+error[XMLHttpRequest.status]);
			}else{
				diyAlert("网络错误,错误编码"+XMLHttpRequest.status);
			}
		},
		success:function(obj){

			//更新系统状态
			var systype = obj["type"];
			if (systype == "0" || systype == "1") 
			{

				var weightType = "";
				if(systype == "0")
					weightType = "A称台";
				else
					weightType = "B称台";

				$('#sysstate>span:eq(1)', window.parent.document).html("静态标定" + weightType);

				//设置称台重量
				var platAweight = obj["weightA"];
				var platBweight = obj["weightB"];
				$("#platAweight>span:eq(1)").html(platAweight);
				$("#platBweight>span:eq(1)").html(platBweight);
			}
			else if (systype == "2") 
			{
				$('#sysstate>span:eq(1)', window.parent.document).html("动态有车...");

				//设置车速
				var speed = obj["speed"];
				$("#speed>span:eq(1)").html(speed);


				//设置称台的系数
				var gska = obj["gska"];
				var gskb = obj["gskb"];
				$("#gskavalue>span:eq(1)").html(gska);
				$("#gskbvalue>span:eq(1)").html(gskb);
				
				//设置过称基本信息
				var carmodel = obj["carmodel"];
				var allWeight = obj["weight"];
				var chargeweight = obj["chargeweight"];
				var axleinfo = obj["axleinfo"];
				var axlecount = axleinfo.length;
				$("#allweight>span:eq(1)").html(allWeight);
				$("#axlecount>span:eq(1)").html(axlecount);
				
				//设置轴信息
				setAxleinfo(axleinfo, axlecount);

				//更新设备信息
				var lightstat = "0"
                var seninfos = obj["seninfo"];

				updateDevState(lightstat,seninfos);

				var date = new Date();
			
				var tr = ""
				if (tablecolor == 0)
				{
					tr = $("<tr " + "class='info'" + " onClick='tableClick(this)'></tr>");
					tablecolor = 1;
				}
				else
				{
					tr = $("<tr " + "class='warning'" + " onClick='tableClick(this)'></tr>");
					tablecolor = 0;
				}
				var td1 = $("<td>"+1+"</td>");
				var td2 = $("<td>"+(axlecount)+"</td>");
                var td3 = $("<td>"+(speed)+"</td>");
				var td4 = $("<td>"+(carmodel)+"</td>");
				var td5 = $("<td>"+(allWeight)+"</td>");
				var td6 = $("<td>"+(chargeweight)+"</td>");
				var td7 = $("<td>"+(date.pattern("yyyy-MM-dd HH:mm:ss"))+"</td>");
				tr.append(td1).append(td2).append(td3).append(td4).append(td5).append(td6).append(td7);
				$("#table_detailinfo tr:first").after(tr);   //插入数据到表格首行

				//更新全局tableInfos信息
				tableInfos.unshift(obj);
				if (tableInfos.length > 15) {
					tableInfos.pop();
					$("#table_detailinfo tr:last").remove(); //删除表格最后一行数据
				}
				
				var tblen = $('#table_detailinfo tr').length;
				for(var i = 2; i <= tblen; i++)
				{
					$('#table_detailinfo tr:eq('+i+') td:first').html(i);
				}
				
			} 
			else if (systype == "3") 
			{
				$('#sysstate>span:eq(1)', window.parent.document).html("动态无车...");

				//设置称台的系数
				var gska = obj["gska"];
				var gskb = obj["gskb"];
				$("#gskavalue>span:eq(1)").html(gska);
                $("#gskbvalue>span:eq(1)").html(gskb);
				
				//设置称台重量
				var platAweight = obj["weightA"];
				var platBweight = obj["weightB"];
				$("#platAweight>span:eq(1)").html(platAweight);
				$("#platBweight>span:eq(1)").html(platBweight);

				// 更新传感器
				var seninfos = obj["seninfo"];
				var lightstat = obj["light"];

				updateDevState(lightstat,seninfos);
				
			}
		},
		complete:function(XMLHttpRequest){
			systemStateTimeout = setTimeout("systemState();",reloadTime);
		}
	});
}

$(function(){ //当文档加载完成就执行

	//设置系统状态信息
	systemState();
	
	//设置开始静态标定点击事件
	$('#button_startcheck').click(function(event) {
		// 开始标定哪一个称台
		var whichOneSelected = $('#selectpicker').val();

		startStatic(whichOneSelected);
	});

	//设置结束静态标定点击事件
	$('#button_stopcheck').click(function(event) {
		var weight = Number($("#allweight>span:eq(1)").html());
		toStartDialog(weight);
	});

	//设置称台置零按钮点击事件
	$("#button_setzero").click(function(){
		setZero(currentweightType);
	});

    //开始动态标定
    $('#begin-get-speed-key').click(function(event) {
        showChooseAxesNumModel();
    });
});

// 弹出选择轴对话框
function showChooseAxesNumModel() {
    var beginModal = getBootStrapModel("choose-axes-num");
    var axesNum = 2;

    beginModal.find("div.modal-header h4>span:eq(1)").html("选择轴数");
    beginModal.on("hidden.bs.modal",function(){
        beginModal.remove();
    });
    var form = $("<form class='form-horizontal'></form>");
    var chooseInfo = $("<div class='form-group'>" +
        "<label for='choose-num' class='col-sm-2 control-label'>轴数</label>" +
        "<div class='col-sm-10'>" +
        "<select class='form-control' id='choose-num'>" +
        "<option>" + 2 + "</option>" +
        "<option>" + 3 + "</option>" +
        "<option>" + 4 + "</option>" +
        "<option>" + 5 + "</option>" +
        "<option>" + 6 + "</option>" +
        "</select>" +
        "</div>"+
        "</div>");
    form.append(chooseInfo);
    beginModal.find(".modal-body").html(form);
    beginModal.find(".modal-footer button:eq(1)").deviceEvent("click",function(){
        currentAxelNum = $("#choose-axes-num #choose-num").val();


        $.ajax({
            url : "/Meter/GetSpeedKey",
            type:"GET",
            dataType:"json",
            data:{
                axlecount:currentAxelNum,
            },
            error:function(XMLHttpRequest, textStatus, errorThrown){
                if(error[XMLHttpRequest.status]){
                    diyAlert("网络错误,无法连接服务器,错误编码"+XMLHttpRequest.status+":"+error[XMLHttpRequest.status]);
                }else{
                    diyAlert("网络错误,无法连接服务器,错误编码"+XMLHttpRequest.status);
                }
            },
            success:function(obj){
                beginModal.modal("hide");

                var keys = Object.keys(obj);
                // 删除表格后多少行
                $('#sm-speed-table tr:gt(0)').remove();

                for (var i = 0; i < keys.length;i++) {
                    var tmpKey = keys[i];
                    var tmpValue = obj[tmpKey];
                    var newRow = "<tr><td>" + (tmpKey) +"</td><td onClick='td_click(this)'>" + (tmpValue) +"</td></tr>";
                    $('#sm-speed-table tr:last').after(newRow);
                }

            },
            complete:function(XMLHttpRequest){
                loadRemove();
            }
        });
    });
    beginModal.modal("show");
}

function td_click(value){
    var speed = Number($(value).prev().text());
    var key = Number($(value).text());
    showEditSpeedModal(speed,key,value);
}

function showEditSpeedModal(pSpeed,pKey,sender) {
    var beginModal = getBootStrapModel("edit-speed");

    beginModal.find("div.modal-header h4>span:eq(1)").html("确认补偿系统");
    beginModal.on("hidden.bs.modal",function(){
        beginModal.remove();
    });
    var form = $("<form class='form-horizontal'></form>");
    var realinfo = $("<div class='form-group'>" +
        "<label for='real_speed' class='col-sm-2 control-label'>轴速"+(pSpeed)+"</label>" +
        "<div class='col-sm-10'>" +
        "<input class='form-control' id='real_key' type='number' value='" + pKey + "' />" +
        "</div>"+
        "</div>");
    form.append(realinfo);
    beginModal.find(".modal-body").html(form);
    beginModal.find(".modal-footer button:eq(1)").deviceEvent("click",function(){

        var realkey = $("#edit-speed #real_key").val();

        $.ajax({
            url : "/Meter/SetSpeedKey",
            type:"GET",
            dataType:"json",
            data:{
                axlecount:currentAxelNum,
                speed: pSpeed,
                key: realkey
            },
            error:function(XMLHttpRequest, textStatus, errorThrown)
            {
                if(error[XMLHttpRequest.status]){
                    diyAlert("网络错误,错误编码"+XMLHttpRequest.status+":"+error[XMLHttpRequest.status]);
                }else{
                    diyAlert("网络错误,错误编码"+XMLHttpRequest.status);
                }
            },
            success:function(obj)
            {

                if(obj.result && obj.result == 0){
                    diyAlert("设置速度补偿系数成功");
                    // 保留六位
                    realkey = Number(realkey).toFixed(6);
                    $(sender).text(realkey);
                }else{
                    diyAlert("设置速度补偿系数失败，原因："+obj.errmsg);
                }

                beginModal.modal("hide");
            },
            complete:function(XMLHttpRequest){

                loadRemove();
            }
        });
    });
    beginModal.modal("show");
}