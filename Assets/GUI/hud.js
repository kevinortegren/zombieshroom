var announcements = [];
function Announce(p_message, p_fadeout)
{
	// If there are existing announcements, push it in the queue
	if(announcements.length > 0)
	{
		// If the last announcement can be overwritten, do it!
		if(announcements[announcements.length-1][1] < 0)
		{
			announcements[announcements.length-1] = [p_message, p_fadeout];
			// If that was the only announcement, show the new one
			if(announcements.length-1 == 0)
				AnnounceNext();
		}
		else
		{
			announcements.push([p_message, p_fadeout]);
		}
	}
	// Else, push into queue and show it
	else
	{
			announcements.push([p_message, p_fadeout]);
			AnnounceNext();
	}
}
function AnnounceNext()
{
	if(announcements.length == 0)
		return;
	$("#Announcement").html(announcements[0][0]);
	$("#Announcement").fadeIn(0, function(){
		if(announcements[0][1]-0.5 >= 0)
			setTimeout(function(){
				$("#Announcement").fadeOut(500, function(){
					announcements.splice(0,1);
					AnnounceNext();
				})
			},
			announcements[0][1]*1000-500);
	});
}
function AddMessage(p_message)
{
	$( "<a>" + p_message + "<br></a>").appendTo( $("#chatlog") ).delay(5000).fadeOut(500);
	$("#chatlog").scrollTop($("#chatlog")[0].scrollHeight);
}
function KillAnnouncement(p_murderer, p_victim, p_skill)
{
  var msg;
  if(p_murderer)
    msg = $("<div>" + p_murderer + " <img src='" + p_skill + ".png'> " + p_victim + "</div>");
  else
    msg = $("<div><img src='RIP.png'> " + p_victim + "</div>");
	msg.appendTo( $("#KillAnnouncementContainer") ).delay(3000).fadeOut(500);
}
function Send(event)
{
	$('#chatinput').blur();
	HUD.Send($("#chatinput").val());
	$("#chatinput").val("");
	event.preventDefault();
	event.stopPropagation();
}
function SetDeathScreen(p_visible)
{
	$("#deathscreen").css("display", (p_visible?"block":"none"));
}
function SetScoreScreen(p_visible)
{
	$("#scorescreen").css("display", (p_visible?"table":"none"));
}
function UpdateScoreScreen(p_localTeam, p_localName, p_list)
{
	var blue, bluetitle;
	var red, redtitle;
	
	$("#BlueScoreTable tbody > tr").remove();
	$("#RedScoreTable tbody > tr").remove();
	$("#SpectatorTable tbody > tr").remove();
	
	for(var player in p_list)
	{
		if(!p_list[player])
			continue;
		var team;
		if(p_list[player][0] == 0)
			team = $("#SpectatorTable tbody");
		else if(p_list[player][0] == 1)
			team = $("#BlueScoreTable tbody");
		else if(p_list[player][0] == 2)
			team = $("#RedScoreTable tbody");
		else
			continue;
		
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

$(document).keydown(function(event)
{
	if(!$('#chatinput').is(":focus") && event.keyCode == 13) // Enter
	{
		$('#chatinput').focus();
	}
	else if(event.keyCode == 27) // Esc
	{
		$('#chatinput').blur();
	}
	else if(event.keyCode == 33) // PgUp
	{
		$('#chatlog').scrollTop($('#chatlog').scrollTop() - $('#chatlog').height()/2);
	}
	else if(event.keyCode == 34) // PgDown
	{
		$('#chatlog').scrollTop($('#chatlog').scrollTop() + $('#chatlog').height()/2);
	}
	else
		return true;
});

function OnLoad()
{
	$('#chatinput').blur(function(){ HUD.SetFocus(false); });
	$('#chatinput').focus(function(){ HUD.SetFocus(true); });
	$('#chatinput').keydown(function()
	{
		if(event.keyCode == 13)
			Send(event);
		else
			return true;
	});
}

function Set(p_id, p_value)
{
	var value = p_value;
	if(p_id == "TimeLeft")
	{
		var time = parseFloat(value);
		// If positive, use floor to get minutes, otherwise use ceil (to round down/up towards zero)
		var minutes = time > 0 ? Math.floor(time / 60) : Math.ceil(time / 60);
		var seconds = Math.abs(time - (minutes * 60));
		if(minutes < 10 && minutes >= 0)
			minutes = "0" + minutes;
		if(seconds < 10)
			seconds = "0" + seconds;
		value = "" + minutes + ":" + seconds;
		if(minutes < 2 && minutes >= 0)
			$("#"+p_id).addClass("blink");
		else
			$("#"+p_id).removeClass("blink");
	}
	if(p_id == "Health")
	{
    var oldHealth = parseInt((($("#healthbar>div>div").width()+$("#healthbar>div>div>div").width())/$("#healthbar>div").width())*100);
    var newHealth = parseInt(value);
    DamageIndicator(oldHealth-newHealth);
		// Health (0~100)
		$("#healthbar>div>div").css('width', $("#healthbar>div").width()*((newHealth<100?newHealth:100)/100.0)+"px");
		// Health overflow (100~200)
		$("#healthbar>div>div>div").css('width', $("#healthbar>div").width()*(((newHealth<200?newHealth:200)-100)/100.0)+"px");
    return;
	}
	if(p_id == "IsDead")
	{
		SetDeathScreen(p_value == "true");
		return;
	}
	if(p_id == "EndGame")
	{
		SetDeathScreen(p_value == "true");
	}
	if(p_id == "ShowScore")
	{
		SetScoreScreen(p_value == "true");
	}
	if(p_id == "ChargeBarValue")
	{
		if(value == 0)
		{
			$("#chargebar>div").fadeOut(500); // fade out
		}
		else
		{
			$("#chargebar div").stop(); // stop any ongoing animations
			$("#chargebar div").css("display", "block"); // ensure the bar is visible
			//$("#chargebar>div>div").animate({width: ($("#chargebar>div").width()*value)+"px"},2000,"linear",function(){$(this).parent().fadeOut(500);});
			$("#chargebar>div>div").css("width", ($("#chargebar>div").width()*value)+"px"); // resize the charge bar
			// If charge reaches end in either direction, start a fadeout - it'll be aborted if a channel is initiated after charge
		}
	}
	$("#"+p_id).html(value);
}

function SetAbility(p_slot, p_ability)
{
	$("#slot"+p_slot+">div.icon").css("background-image", "url('"+p_ability+".png')");
}

function SetAbilityFocus(p_slot)
{
  if($(".slotselected") == $("#slot"+p_slot))
    return;
  $(".slotselected>div.focus").stop();
	$(".slotselected").removeClass("slotselected");
	$("#slot"+p_slot).addClass("slotselected");
  
  $(".slotselected>div.focus").animate({transform: 'rotate(180deg)'}, 3000);
}
function SetCooldown(p_slot, p_percent)
{
	var slot = $("#slot"+p_slot+">div.cooldown");
	if(p_percent > 0)
	{
		slot.css("background-position", "50% 50%");
		slot.css("background-size", p_percent*56+"px");
	}
	else
		slot.css("background-position", "50% 64px");
}

function SetCharges(p_slot, p_charges)
{
  var slot = $("#slot"+p_slot+">div.icon");
  if(p_charges > 0)
    slot.html(p_charges);
  else
    slot.html("");
}

function DamageIndicator(p_damage)
{
  if(p_damage > 0)
  {
    var opacity = (p_damage/10.0);
    $("#damageTaken").stop().css("opacity", opacity).animate({opacity: 0}, 1000 * opacity);
  }
  // else if(p_damage < 0)
  // {
    // $("#damageTaken").stop().css("opacity", 0);
  // }
}

$(document).ready(function(){
	SetAbilityFocus(1);
    Set("Health", 0);
    //Set("Health", 40);
	//SetAbility(1, "FireBall");
	// StartCooldown(1, 3);
	//UpdateScoreScreen(2, "doqunbop", [[2,"doqunbop",1,11],[1,"The Enemy",11,2],[1,"The Enemy2",0,2],[2,"The Ally",3,0]]);
	//Set("ShowScore", "false");
	//UpdateScoreScreen(2,'doqunbop',[[1,'Player',2,2][2,'doqunbop',1,0]]);
  // DamageIndicator(40);
   //setTimeout("DamageIndicator(40);", 2000);
	//Set("ChargeBarValue", 1);
	// Announce("Waiting for players...", -1);
  //SetCharges(1,3);
	// setTimeout("Announce('5',1);Announce('4',1);Announce('3',1);Announce('2',1);Announce('1',1);Announce('May the roots be with you!',3);", 3000);
	//$("#healthbar>div>div").css('width', $("#healthbar>div").width()*(99/100.0)+"px");
	//Set('Health', 166);
  // KillAnnouncement('Bulbasaur', 'Ekans', 'AbilityBall');
  // KillAnnouncement('','Bulbasaur', '');
});
