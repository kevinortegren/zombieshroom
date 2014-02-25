function HideTeamSelect()
{
		$("#team-select").css("display", "none");
		$("#ingame-menu").css("display", "table");
}
function ShowTeamSelect()
{
		$("#ingame-menu").css("display", "none");
		$("#team-select").css("display", "table");
}

$(document).ready(function() {

	$("#ingame-return").mouseup(function() {
		Menu.Return();
	} );
	  
	$("#ingame-settings").mouseup(function() {
		$("#ingame-menu").css("display", "none");
		$("#settings-menu").css("display", "table");
		JSGlobal.LoadSettings();
	} );
	$("#ingame-change-team").click(ShowTeamSelect);

	$("#ingame-exit").mouseup(function() {
		Menu.Exit();
	} );
		
	$("#team-select-blue").click(function() {
		Menu.SelectTeam(1);
		HideTeamSelect();
	});
	$("#team-select-red").click(function() {
		Menu.SelectTeam(2);
		HideTeamSelect();
	});
	$("#team-select-spectate").click(function() {
		Menu.SelectTeam(0);
		HideTeamSelect();
	});
	$("#team-select-back").click(HideTeamSelect);
  
} );
  
  
  