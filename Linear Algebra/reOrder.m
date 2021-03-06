function [Ha Qo] = reOrder(Ha);
s = size(Ha,1);
Qo = eye(s);
for i=1:s-1
	d = diag(Ha(i:s,i:s));
	[num itr] = max(d);
	itr = itr + i-1;
	
	M = eye(s);
	M(itr,itr) = 0;
	M(i,i) = 0;
	M(itr,i) = 1;
	M(i,itr) = 1;
	Ha = M*Ha*M';
	Qo = Qo*M';
end
M = eye(s);
Mr = 0*eye(s/2);
for i=1:s/2
	Mr(i,s/2-i+1) = 1;
end
M(s/2+1:s,s/2+1:s) = Mr;
Ha = M*Ha*M;
Qo = Qo*M;
