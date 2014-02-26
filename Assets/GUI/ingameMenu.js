var teamSize = [0,0,0];

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
function UpdateScoreScreen(p_localTeam, p_localName, p_list)
{
	var blue, bluetitle;
	var red, redtitle;
	$("#BlueScoreTable tbody > tr").remove();
	$("#RedScoreTable tbody > tr").remove();
	$("#SpectatorTable tbody > tr").remove();
	teamSize = [0,0,0];
	
	for(var player in p_list)
	{
		if(!p_list[player])
			continue;
		var team;
		if(p_list[player][0] == 0)
			team = $("#SpectatorTable");
		else if(p_list[player][0] == 1)
			team = $("#BlueScoreTable");
		else if(p_list[player][0] == 2)
			team = $("#RedScoreTable");
		else
			continue;
		teamSize[p_list[player][0]]++;
		
		team.append("<tr"+(p_list[player][1]==p_localName?" class='highlight'":"")+">"
				+(p_list[player][0] != 0
					?"<td class='textright'>"+p_list[player][1]+"</td>"
						+"<td class='textcenter'>"+p_list[player][2]+"</td>"
						+"<td class='textcenter'>"+p_list[player][3]+"</td>"
					:"<td class='textcenter'>"+p_list[player][1]+"</td>"
				)
			+"</tr>");
	}
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
		if(teamSize[1] > teamSize[2])
			return;
		Menu.SelectTeam(1);
		HideTeamSelect();
	});
	$("#team-select-red").click(function() {
		if(teamSize[2] > teamSize[1])
			return;
		Menu.SelectTeam(2);
		HideTeamSelect();
	});
	$("#team-select-spectate").click(function() {
		Menu.SelectTeam(0);
		HideTeamSelect();
	});
	$("#team-select-back").click(HideTeamSelect);
  
} );
  
  
  