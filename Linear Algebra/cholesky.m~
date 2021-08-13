function R = cholesky(A)
m = size(A,2);
R = 0*A;
for i=1:m
	for j=1:m
		if (j>=i)
			R(i,j) = A(i,j);
		end
	end
end
for k=1:m
	for j=k+1:m
		R(j,j:m) = R(j,j:m) - R(k,j)/R(k,k)*R(k,j:m);
	end
	R(k,k:m) = R(k,k:m)/sqrt(R(k,k));
end
