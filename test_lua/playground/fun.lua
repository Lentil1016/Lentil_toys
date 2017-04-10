function max(num1, num2)
	if(num1 > num2) then
		result = num1;
	else
		result = num2;
	end

	return result
end
function ave(...)
	result = 0
	local arg={...}
	for i, v in ipairs(arg) do
		result = result + v
	end
	print("input number : ".. #arg)
	return result/#arg
end

print("average of 1,2,3,4,5,6,7,8 is : ".. ave(1,2,3,4,5,6,7,8));
print("test who is bigger, 10 or 4? " .. max(10,4));
