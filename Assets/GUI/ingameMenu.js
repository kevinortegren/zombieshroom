$(document).ready(function() {

	$("#ingame-return").mouseup(function() {
		Menu.Return();
	  } );
	  
	  $("#ingame-settings").mouseup(function() {
		$("#ingame-menu").css("display", "none");
		$("#settings-menu").css("display", "table");
		JSGlobal.LoadSettings();
	  } );

	$("#ingame-exit").mouseup(function() {
		Menu.Exit();
	  } );
  
} );
  
  
  