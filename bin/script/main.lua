
dofile "./script/xxs/include.lua"

function main()
	print("==xxsadd:", add(2,3));
	print("==background==:", background);
	if background then 
		print("background:r",background.r," g:", background.g," b:", background.b);
	end
	print("==cppadd:", add2(1,3));
	print("==cppsub:", sub2(5,2));
end