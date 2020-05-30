function write2cpp(A)
fid = fopen('Matlab2CRS.txt','w');

if (fid < 0)
    fprintf('Could not open file\n');
    return;
end

[row,col,val] = find(A');

fprintf(fid,'%d\n',length(col));
for i=1:length(col)
    fprintf(fid,'%d\t%d\t%.16e\n',col(i),row(i),val(i));
end

fclose(fid);
fprintf('Successfully done\n');
end