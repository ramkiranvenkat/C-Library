function [q r] = QR(A)
v = A;
for i=1:size(A,2)
	r(i,i) = norm(v(:,i));
	q(:,i) = v(:,i)/r(i,i);
	for j = i+1:size(A,2)
		r(i,j) = q(:,i)'*v(:,j);
		v(:,j) = v(:,j) - q(:,i)*q(:,i)'*v(:,j);
	end 
end
