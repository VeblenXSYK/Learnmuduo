var weightype_ = "1";

$(function () {
    $(".dropdown-menu li a").click(function(){
        $(this).parents(".dropdown").find('.btn').html($(this).text() + ' <span class="caret"></span>');
        $(this).parents(".dropdown").find('.btn').val($(this).data('value'));
    });

    $('#start-test-btn').click(function () {
        startTest();
    });

    $('#stop-test-btn').click(function () {
        stopTest();
    });

    $(".downloadButton").click(function(){
        var href = $('.downloadLink').attr('href');
        window.parent.location.href = href;
    });
    
    fetchTestStatus();



    // 读取json数据
    // $.getJSON("../data/truckscale.json",function(data){
    //     var dataArr = data.data;
    //
    //     var firstData = dataArr[0];
    //
    //     var firstDataArry = firstData.split(" ");
    //
    //     var formatData = [];
    //     for (var i = 0; i < dataArr.length; i++) {
    //         var tmpData = dataArr[i].split(" ");
    //         var tmpArr = [];
    //
    //         for (var j=0; j < firstDataArry.length; j++) {
    //             var nexFixData = tmpData[j] - firstDataArry[j];
    //             tmpArr.push(nexFixData);
    //         }
    //         formatData.push(tmpArr);
    //     }
    //
    //     var dataSeries = [];
    //
    //     for (var i=0; i<firstDataArry.length;i++) {
    //         var tmpObj = {
    //             data:[],
    //             type: 'line'
    //         };
    //
    //         var nestData = [];
    //
    //         for (var j=0;j<10000;j++) {
    //             nestData.push(formatData[j][i]);
    //         }
    //
    //         tmpObj.data = nestData;
    //         dataSeries.push(tmpObj);
    //     }
    //
    //     // // 基于准备好的dom，初始化echarts实例
    //     // var myChart = echarts.init(document.getElementById('chart-containner'));
    //     //
    //     // // 指定图表的配置项和数据
    //     // var option = {
    //     //     title: {
    //     //     },
    //     //     tooltip: {},
    //     //     legend: {
    //     //     },
    //     //     xAxis: {
    //     //         type: 'time',
    //     //         name: '时间',
    //     //         axisLabel: {
    //     //             show: false
    //     //         },
    //     //         axisTick: {
    //     //             show: false
    //     //         }
    //     //     },
    //     //     yAxis: {
    //     //         type: 'value'
    //     //     },
    //     //     series: dataSeries,
    //     //     dataZoom: [
    //     //         {
    //     //             type: 'inside',
    //     //             orient: 'vertical',
    //     //             start:70,
    //     //             end:0
    //     //         },{
    //     //             type: 'slider',
    //     //             orient: 'vertical',
    //     //         },{
    //     //             type: 'slider',
    //     //             orient: 'horizontal'
    //     //         }
    //     //     ]
    //     // };
    //     //
    //     // // 使用刚指定的配置项和数据显示图表。
    //     // myChart.setOption(option);
    //
    //
    //
    //
    // })






});


function startTest() {
    $.ajax({
        url : "/System/StartTest",
        type:"GET",
        dataType:"json",
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
                diyAlert("开始测试成功");

                $('#sysstate>span:eq(1)', window.parent.document).html("正在测试");
                setButtonUnable();

            } else if (obj.result && obj.result == 1) {
                diyAlert("结束系统测试");
            } else{
                diyAlert("设置速度补偿系数失败，原因："+obj.errmsg);
            }
        }
    });
}

function stopTest() {
    $.ajax({
        url : "/System/StopTest",
        type:"GET",
        dataType:"json",
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
            if(obj.weightype){
                weightype_ = obj.weightype

                setButtonAble();
                $('#sysstate>span:eq(1)', window.parent.document).html("未测试");

                if (weightype_ == 1) {
                    $('#download-btn').prop('href',"../data/truckscale.dat");
                } else if (weightype_ ==2 ) {
                    $('#download-btn').prop('href',"../data/axlegroup.dat");
                }
            }  else{
                diyAlert("结束测试失败，原因："+obj.errmsg);
            }
        }
    });
}

function fetchTestStatus() {
    $.ajax({
        url : "/System/TestStatus",
        type:"GET",
        dataType:"json",
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
                if (obj.status === 0) {
                    $('#sysstate>span:eq(1)', window.parent.document).html("未测试");

                    setButtonAble();
                } else if (obj.status === 1){
                    $('#sysstate>span:eq(1)', window.parent.document).html("正在测试");
                    setButtonUnable();
                } else {
                    diyAlert("获取测试状态失败，原因：" + obj.errmsg);
                }
        }
    });

}

function setButtonUnable() {
    $('#start-test-btn').prop('disabled',true);
    $('#download-btn').prop('disabled', true);
}

function setButtonAble() {
    $('#start-test-btn').prop('disabled',false);
    $('#download-btn').prop('disabled', false);
}