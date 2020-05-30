function A = readFromCpp()
A = load('CRS2Matlab.txt');
A = spconvert(A);
end