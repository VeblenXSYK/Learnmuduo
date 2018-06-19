var canEdit = false;

$(function () {

    $('#sysstate>span:eq(1)', window.parent.document).html("设置");

    fetchSysConfig();
    setUnEditable();
    setupInputOberserver();

    $('#high_setting_edit').click(function (evt) {
        high_setting_edit();
    });

    $('#high_setting_save').click(function (evt) {
        setSysConfig();
    });

    $('a[data-toggle="tab"]').on('shown.bs.tab', function (e) {
        var target = $(e.target).attr("href");
        if ((target == '#messages')) {

        } else if (target == '#verify') {
            checkDeviceIsValid();
        }
    });

    $('#upload-btn').click(function (evt) {
        uploadLicense();
    });

    $("input:file").change(function () {
        var fileName = $(this).val();
        $("#upload-btn-input").html(fileName);
    });


});

function fetchSysConfig() {
    $.ajax({
        url: "/System/GetSysConfig",
        type: "GET",
        dataType: "json",
        error: function (XMLHttpRequest, textStatus, errorThrown) {
            if (error[XMLHttpRequest.status]) {
                diyAlert("网络错误,无法连接服务器,错误编码" + XMLHttpRequest.status + ":" + error[XMLHttpRequest.status]);
            } else {
                diyAlert("网络错误,无法连接服务器,错误编码" + XMLHttpRequest.status);
            }
        },
        success: function (obj) {
            var deviceID = obj["devid"];
            var weightype = obj["weightype"];
            var senum = obj["senum"];
            var remoteip = obj["remoteip"];
            var jsonport = obj["jsonport"];
            var recordport = obj["recordport"];

            $('#device_id').val(deviceID);
            $('#channel_hop_interval').val(weightype);
            $('#lowest_power').val(senum);
            $('#max_hijack_stat').val(remoteip);
            $('#disk_threshold').val(jsonport);
            $('#port_sensor').val(recordport);
        }
    });
}

function setUnEditable() {
    canEdit = false;

    $('#device_id').prop('disabled', true);
    $('#channel_hop_interval').prop('disabled', true);
    $('#lowest_power').prop('disabled', true);
    $('#max_hijack_stat').prop('disabled', true);
    $('#disk_threshold').prop('disabled', true);
    $('#port_sensor').prop('disabled', true);

    $('#high_setting_save').prop('disabled', true);
}

function setCanEditable() {
    canEdit = true;

    $('#device_id').prop('disabled', false);
    $('#channel_hop_interval').prop('disabled', false);
    $('#lowest_power').prop('disabled', false);
    $('#max_hijack_stat').prop('disabled', false);
    $('#disk_threshold').prop('disabled', false);
    $('#port_sensor').prop('disabled', false);

    $('#high_setting_save').prop('disabled', false);
}

function high_setting_edit() {
    setCanEditable();
}

function updateSettingBtn() {
    var devId = $('#device_id').val();
    var weightype = $('#channel_hop_interval').val();
    var senum = $('#lowest_power').val();
    var remoteip = $('#max_hijack_stat').val();
    var jsonport = $('#disk_threshold').val();
    var recordport = $('#port_sensor').val();

    if (!devId) {
        $('#high_setting_save').prop('disabled', true);
        return;
    }

    if (!weightype) {
        $('#high_setting_save').prop('disabled', true);
        return;
    }

    if (!senum) {
        $('#high_setting_save').prop('disabled', true);
        return;
    }

    if (!remoteip) {
        $('#high_setting_save').prop('disabled', true);
        return;
    }

    if (!jsonport) {
        $('#high_setting_save').prop('disabled', true);
        return;
    }

    if (!recordport) {
        $('#high_setting_save').prop('disabled', true);
        return;
    }

    $('#high_setting_save').prop('disabled', false);
}

function setupInputOberserver() {
    $('#device_id').change(function (evt) {
        updateSettingBtn();
    });
    $('#channel_hop_interval').change(function (evt) {
        updateSettingBtn();
    });
    $('#lowest_power').change(function (evt) {
        updateSettingBtn();
    });
    $('#max_hijack_stat').change(function (evt) {
        updateSettingBtn();
    });
    $('#disk_threshold').change(function (evt) {
        updateSettingBtn();
    });
    $('#port_sensor').change(function (evt) {
        updateSettingBtn();
    });
}

function setSysConfig() {
    var devId = encodeURIComponent($('#device_id').val());
    var weightype = $('#channel_hop_interval').val();
    var senum = $('#lowest_power').val();
    var remoteip = $('#max_hijack_stat').val();
    var jsonport = $('#disk_threshold').val();
    var recordport = $('#port_sensor').val();


    $.ajax({
        url: "/System/SetSysConfig",
        type: "GET",
        dataType: "json",
        data: {
            "devid": devId,
            "weightype": weightype,
            "senum": senum,
            "remoteip": remoteip,
            "jsonport": jsonport,
            "recordport": recordport
        },
        error: function (XMLHttpRequest, textStatus, errorThrown) {
            if (error[XMLHttpRequest.status]) {
                diyAlert("网络错误,无法连接服务器,错误编码" + XMLHttpRequest.status + ":" + error[XMLHttpRequest.status]);
            } else {
                diyAlert("网络错误,无法连接服务器,错误编码" + XMLHttpRequest.status);
            }
        },
        success: function (obj) {
            if (obj.result && obj.result == 0) {
                diyAlert("系统设置成功");

            } else {
                diyAlert("系统设置失败，原因：" + obj.errmsg);
            }
        }
    });
}

function checkDeviceIsValid() {
    $.ajax({
        url: "/System/GetLegalInfo",
        type: "GET",
        dataType: "json",
        error: function (XMLHttpRequest, textStatus, errorThrown) {
            if (error[XMLHttpRequest.status]) {
                diyAlert("网络错误,无法连接服务器,错误编码" + XMLHttpRequest.status + ":" + error[XMLHttpRequest.status]);
            } else {
                diyAlert("网络错误,无法连接服务器,错误编码" + XMLHttpRequest.status);
            }
        },
        success: function (obj) {
            if (obj.result && obj.result == 0) {
                diyAlert("设备已验证");

                deviceSetupValid();

            } else if (obj.result && obj.result == 1) {

                diyAlert("设备未验证，请上传授权license");
                deviceSetupInvalid();

            } else {
                diyAlert("设备验证失败，原因：" + obj.errmsg);
                deviceSetupInvalid();
            }
        }
    });
}

function deviceSetupValid() {
    var successImg = "../images/status-success.png";

    $('#status-image').attr('src', successImg);
    $('#verify-status').html('设备已验证');
    $('#upload-wraper').hide();
}

function deviceSetupInvalid() {
    var failImg = "../images/status-fail.png";

    $('#status-image').attr('src', failImg);
    $('#verify-status').html('设备未验证，请上传license');
    $('#upload-wraper').show();
}

function uploadLicense() {
    if ($('#upload-license-input').prop('files').length == 0) {
        diyAlert("请选择License！");
        return;
    }

    var licenseFile = $('#upload-license-input').prop('files')[0];

    var upload = new Upload(licenseFile);


    upload.doUpload();

}