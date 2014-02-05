$(document).ready(function() {

	$("#ingame-return").mouseup(function() {
		Menu.Return();
	  } );
	  
	  $("#ingame-settings").mouseup(function() {
		$("#ingame-menu").css("display", "none");
		$("#settings-menu").css("display", "table");
		//LoadSettings();
	  } );

	$("#ingame-exit").mouseup(function() {
		Menu.Exit();
	  } );
  
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
  
} );
  
  
  