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
var url_get = "/get?id=";
var url_set = "/set?";
var url_new = "/new";
var url_delete = "/delete?";
var inputNum = 1;

$(document).ready(() => {
  $("#btn-connect").on("click", function () {
    ip = "http://" + $("#ip-value").val();
    pureIp = $("#ip-value").val();
    connetti();
  });
  $("#btn-add").on("click", function () {
    resetForm();
    $("#modal-edit").modal();
  });

  $("#btn-add-modal").on("click", function (event) {
    var data = $("#form-modal ").serialize();
    $.post(ip + url_set, data, function (data) {
      connetti();
    });
    event.preventDefault();
  });
  $("#btn-delete-modal").on("click", function (event) {
    var data = $("#form-modal ").serialize();
    $.post(ip + url_delete, data, function (data) {
      connetti();
    });
    event.preventDefault();
  });

  $(".toggle-password").on("click", function () {
    $(this).toggleClass("fa-eye fa-eye-slash");
    let input = $($(this).attr("toggle"));
    if (input.attr("type") == "password") {
      input.attr("type", "text");
    } else {
      input.attr("type", "password");
    }
  });

  $(".dropdown-tipo-class").on("change", function (e) {
    console.log(e);
  });
  $(".dropdown-tipo-class").change(function (e) {
    console.log(e);
  });
  $("select").on("change", function () {
    alert(this.value);
  });
  console.log($("select>option"));

});

function sendNew() {
  $.get(ip + url_new);
}

function connetti() {
  showLoading();
  $("#IP-label").html("");
  $("#IP-label").append("Your IP is: " + pureIp);
  $("#list-all").html("");
  $.get(ip + url_tutti, function (data) {
    compilaCose(data);
    nascondi();
    hideLoading();
  }).fail(function (e) {
    hideLoading();
    nonRaggiungibile();
    console.log(e);
  });
}
function showModal(id) {
  showLoading();
  $.get(ip + url_get + id, function (data) {
    var json = JSON.parse(data);
    $("#autoform").html("");
    $.each(json, function (key, value) {
      var idEl = "input-row-" + value.n_seq;
      var row = $("#input-row-shape").clone();
      row.attr("id", idEl);
      $("#autoform").append(row);

      var input = $("#" + idEl + ">#input-modal-shape");
      input.attr("id", "input-modal-" + value.n_seq);
      input.attr("value", value.content);

      var dropdown = $("#" + idEl + ">div>#dropdown-tipo-shape");
      dropdown.attr("id", "dropdown-tipo-" + value.n_seq);
      dropdown.attr("toggle", "#input-modal-" + value.n_seq);
      dropdown.val(value.type);
      dropdown[0].addEventListener("change", (event) => {
        var input = $($(event.target).attr("toggle"));
        dropdownCambiato(input, $(event.target));
      });
      dropdownCambiato(input, dropdown);

      var btnRemove = $("#" + idEl + ">div>#span-remove-row-shape");
      btnRemove.attr("id", "span-remove-row-" + value.n_seq);
      btnRemove.data("n_seq", value.n_seq);
      btnRemove.children().data("n_seq", value.n_seq);
      btnRemove.data("p_id", id);
      btnRemove.children().data("p_id", id);
      console.log(btnRemove);
      btnRemove[0].addEventListener("click", (event) => {
        var btn = $(event.target);
        console.log(btn);
        rimuoviriga(btn.data("n_seq"), btn.data("p_id"));
      });
    });
    $("#modal-edit").modal();
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
        value.title +
        "</td><td>" +
        value.email +
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
  console.log($("#modal-edit"));
  $("#form-modal").trigger("reset");
}

function loadAutoform() {
  var t = $("#input-row-shape").clone();
  console.log(t);
  t.attr("id", "input-row-temp");
  $("#autoform").append(t);
}
function dropdownCambiato(input, targhet) {
  console.log(input);
  console.log(targhet);
  var sel = $("#" + targhet.attr("id") + ">option:selected").attr("value");
  console.log(sel);
  switch (sel) {
    case "text":
      input.attr("type", "text");
      break;
    case "password":
      input.attr("type", "password");
      break;
    case "hidden":
      input.attr("type", "hidden");
      break;
  }
}
function rimuoviriga(n_seq, n_id) {
  $.post(ip + url_delete, "n_seq=" + n_seq + "&n_id=" + n_id, function (data) {
    alert(data);
  });
}
