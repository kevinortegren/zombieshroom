var JSGlobal = {};

$(document).ready(function() {
  
  // Settings menu
  $("#settings-player").click(function() {
    $("#settings-menu").css("display", "none");
    $("#player-settings-menu").css("display", "table");
  } );
  $("#settings-graphics").click(function() {
    $("#settings-menu").css("display", "none");
    $("#graphics-settings-menu").css("display", "table");
  } );
  $("#settings-sound").click(function() {
    ShowError("Not yet implemented! D:");
  } );
  $("#settings-controls").click(function() {
    ShowError("Not yet implemented! D:");
  } );
  $("#settings-back").click(function() {
    $("#settings-menu").css("display", "none");
    $("#ingame-menu").css("display", "table");
  } );
  // Player settings
  JSGlobal.LoadSettings = function () {
    var settings = Menu.RequestSettings();
    for(var key in settings) {
      if(key == "settings-fullscreen")
        $("input[name=settings-fullscreen]").filter("[value="+settings[key]+"]").prop("checked", true);
      else if(key == "settings-resolution-list")
      {
        $("#settings-resolution").empty();
        eval(settings[key]).forEach(function(resolution) {
          $("#settings-resolution").append("<option value='"+resolution+"'>"+resolution+"</option>");
        });
      }
      else
        $("#"+key).val(settings[key]);
    }
  }
  $("#player-settings-back").click(function() {
    $("#player-settings-menu").css("display", "none");
    $("#settings-menu").css("display", "table");
    JSGlobal.LoadSettings();
  } );
  $("#player-settings-save").click(function() {
    var settings = {};
    settings["settings-player-name"] = $("#settings-player-name").val();
    Menu.SaveSettings(settings);
    $("#player-settings-menu").css("display", "none");
    $("#settings-menu").css("display", "table");
  } );
  
  $("#graphics-settings-back").click(function() {
    $("#graphics-settings-menu").css("display", "none");
    $("#settings-menu").css("display", "table");
    JSGlobal.LoadSettings();
  } );
  $("#graphics-settings-save").click(function() {
    var settings = {};
    settings["settings-fullscreen"] = $("input[name=settings-fullscreen]:checked").val();
    settings["settings-resolution"] = $("#settings-resolution").val();
    Menu.SaveSettings(settings);
    $("#graphics-settings-menu").css("display", "none");
    $("#settings-menu").css("display", "table");
  } );
  
  
  } );