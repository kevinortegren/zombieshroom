function AddMessage(p_message)
{
	$("#chatlog").html( $("#chatlog").html() + p_message + "<br>");
	$("#chatlog").scrollTop($("#chatlog")[0].scrollHeight);
}
function Send(event)
{
	$('#chatinput').blur();
	ChatSystem.Send($("#chatinput").val());
	$("#chatinput").val("");
	event.preventDefault();
	event.stopPropagation();
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
	else
		return true;
});

function OnLoad()
{
	$('#chatinput').blur(function(){ ChatSystem.SetFocus(false); });
	$('#chatinput').focus(function(){ ChatSystem.SetFocus(true); });
	$('#chatinput').keydown(function()
	{
		if(event.keyCode == 13)
			Send(event);
		else
			return true;
	});
}

function SetHealth(p_health)
{
	
}