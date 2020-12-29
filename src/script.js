function nascondi() {
  document.getElementById("table-place").classList.remove("nascosto");
  document.getElementById("status-bar-not-connected").classList.add("nascosto");
  document.getElementById("status-bar-connected").classList.remove("nascosto");
  document.getElementById("IP-form").classList.add("nascosto");
  document.getElementById("IP-connected").classList.remove("nascosto");
}

var ip = "";
var pureIp = "";
var url_modifica = "";
//var url_tutti = "src/response.text";
var url_tutti = "/all";
//var url_get = "src/3.text;
var url_get = "/get/?id=";
var url_set = "/set/?id=";
var url_new = "/new";

$(document).ready(() => {
  $("#btn-connect").on("click", function () {
    ip = "http://" + $("#ip-value").val();
    pureIp = $("#ip-value").val();
    console.log(ip);
    connetti();
  });
  $("#btn-add").on("click", function () {
    resetForm();
    $("#form-modal").modal();
  });
  $("#input-modal").submit(function (event) {
    console.log(event.targhet);
    $.post(
      ip + url_set + $("#input-modal").serialize().id,
      $("#input-modal").serialize()
    ).done(function (data) {
      alert("Data Loaded: " + data);
    });
    alert("Handler for .submit() called.");
    event.preventDefault();
  });
});

function connetti() {
  showLoading();
  $("#IP-label").html("");
  $("#IP-label").append("Your IP is: " + pureIp);
  $("#list-all").html("");

  /* $.ajax({
    type: "GET",
    url: ip + url_tutti,
    cache: false,
    beforeSend: function (request) {
      request.withCredentials = false;
      request.setRequestHeader(
        "Authorization",
        "Basic " + btoa("admin" + ":" + "password")
      );
    },
    success: function (data) {
      compilaCose(data);
      nascondi();
      hideLoading();
    },
    fail: function (params) {
      alert(params);
    },
  });*/

  $.get(ip + url_tutti, function (data) {
    compilaCose(data);
    nascondi();
    hideLoading();
  })
    .done(function () {})
    .fail(function (e) {
      hideLoading();
      nonRaggiungibile();
      console.log(e);
    })
    .always(function () {});
}
function showModal(id) {
  showLoading();
  console.log($("#form-modal"));
  $.get(ip + url_get + id, function (data) {
    var json = JSON.parse(data);
    $.each(json, function (key, value) {
      $("#input-modal-" + key).val(value);
    });
    $("#form-modal").modal();
    hideLoading();
  }).fail(function (e) {
    hideLoading();
    nonRaggiungibile();
    console.log(e);
  });
}

function compilaCose(data) {
  console.log(data);
  var json = jQuery.parseJSON(data);
  $.each(json, function (index, value) {
    $("#list-all").append(
      //$("#ghost-item").before(
      '<tr class="item-row" id = "item-row-' +
        value.id +
        '"><th scope="row">' +
        value.id +
        "</th><td>" +
        value.name +
        "</td><td>" +
        value.username +
        "</td><td>" +
        value.password +
        '</td><td><button class="btn-sm btn-outline-primary" onclick= "showModal(' +
        value.id +
        ')" >Edit</button></td></tr>'
    );
    console.log(value);
  });

  $(".result").html(json);
}
function nonRaggiungibile() {
  alert("Non riesco a raggiungere il Pino");
}

function showLoading() {
  $("#loading-span").show();
}

function hideLoading() {
  $("#loading-span").hide();
}
function resetForm() {
  console.log($("#form-modal"));
  $("#form-modal").trigger("reset");
}
