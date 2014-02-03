$(document).ready(function() {

	$("#ingame-return").click(function() {
		IngameMenu.Return();
	  } );
	  
	  $("#ingame-settings").click(function() {
		$("#ingame-menu").css("display", "none");
		$("#settings-menu").css("display", "table");
		//LoadSettings();
	  } );

	$("#ingame-exit").click(function() {
		IngameMenu.Exit();
	  } );
  
  // Settings menu
  $("#settings-player").click(function() {
    $("#settings-menu").css("display", "none");
    $("#player-settings-menu").css("display", "table");
  } );
  $("#settings-graphics").click(function() {
    ShowError("Not yet implemented! D:");
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
  LoadSettings = function () {
    var settings = Menu.RequestSettings();
    for(var key in settings)
      $("#"+key).val(settings[key]);
  }
  $("#player-settings-back").click(function() {
    $("#player-settings-menu").css("display", "none");
    $("#settings-menu").css("display", "table");
    LoadSettings();
  } );
  $("#player-settings-save").click(function() {
    var settings = {};
    settings["settings-player-name"] = $("#settings-player-name").val();
    Menu.SaveSettings(settings);
    $("#player-settings-menu").css("display", "none");
    $("#settings-menu").css("display", "table");
  } );
  
  
  
  } );
  
  
  