var Upload = function (file) {
    this.file = file;
};

Upload.prototype.getType = function() {
    return this.file.type;
};
Upload.prototype.getSize = function() {
    return this.file.size;
};
Upload.prototype.getName = function() {
    return this.file.name;
};
Upload.prototype.doUpload = function () {
    var that = this;
    var formData = new FormData();

    // add assoc key values, this will be posts values
    formData.append("filename", this.file, this.getName());
    // formData.append("upload_file", true);

    $.ajax({
        type: "POST",
        url: "/System/Authentication",
        dataType:"json",
        xhr: function () {
            var myXhr = $.ajaxSettings.xhr();
            if (myXhr.upload) {
                myXhr.upload.addEventListener('progress', that.progressHandling, false);
            }
            return myXhr;
        },
        success: function (obj) {
            // your callback here
            if(obj.result && obj.result == 0){
                diyAlert("设备已验证");

                deviceSetupValid();

            } else if (obj.result && obj.result == 1) {

                diyAlert("设备未验证，请上传授权license");
                deviceSetupInvalid();

            }else{
                diyAlert("设备验证失败，原因："+obj.errmsg);
                deviceSetupInvalid();
            }
        },
        error:function(XMLHttpRequest, textStatus, errorThrown){
            if(error[XMLHttpRequest.status]){
                diyAlert("网络错误,无法连接服务器,错误编码"+XMLHttpRequest.status+":"+error[XMLHttpRequest.status]);
            }else{
                diyAlert("网络错误,无法连接服务器,错误编码"+XMLHttpRequest.status);
            }
        },
        async: true,
        data: formData,
        cache: false,
        contentType: false,
        processData: false,
        timeout: 60000
    });
};

Upload.prototype.progressHandling = function (event) {
    var percent = 0;
    var position = event.loaded || event.position;
    var total = event.total;
    var progress_bar_id = "#progress-wrp";
    if (event.lengthComputable) {
        percent = Math.ceil(position / total * 100);
    }
    // update progressbars classes so it fits your code
    $(progress_bar_id + " .progress-bar").css("width", +percent + "%");
    $(progress_bar_id + " .status").text(percent + "%");
};