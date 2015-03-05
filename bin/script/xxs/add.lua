
bkground = {r=10,g=20,b=40,a=50};
width = 11;
height = 22;
function add(x, y)
	return x+y;
end

_G.console = {};

function console:init(objConsole)
	print("console:init");
	console = objConsole;
end
function console:Add(op1, op2)
	return op1+op2;
end
function console:Sub(op1, op2)
	return op1-op2;
end