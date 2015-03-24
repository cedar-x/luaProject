
dofile "./script/xxs/include.lua"

function main()
	print("==lua_add:", add(2,3));
	print("==cpp_background==:", background);
	if background then 
		print("cpp_background:r",background.r," g:", background.g," b:", background.b);
	end
	print("==cpp_add:", add2(1,3));
	print("==cpp_sub:", sub2(5,2));
	-- local c = CTest()
	-- print("c.Add(1, 2) ==> " .. c:Add(1, 2));
	-- local d = CTest()
	-- print("d.Add(4, 5) ==> " .. d:Add(4, 5));
	local ff = Foo(21)
    local v = ff:add(1, 5)
    print("===cpp_class:Foo(3):add(1,4):", v, " value:", ff:getV());
    ff:setV(13);
    print("==value:", ff:getV());
    print("============================")
    local b = {};
    b.a = function(str, cfunc)
    	print("b.a start=========", str);
    	cfunc("this is c function\n");
	end
	-- lua_clib(b.a, "test");
	lua_clib(b.a,"test");
	print("==============================");
	Console:warn("==Console:warn..");
end