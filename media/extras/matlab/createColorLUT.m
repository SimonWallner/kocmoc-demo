% generate a color LUT 3d Texture to use in out-of-engine color
% correction.

function [ image ] = createColorLUT( width, height, depth )
% CREATECOLORLUT
% Create a 3d color LUT texture to is out-of-engine color correction
% color values are in the range on [0, 1]
% @param width The width of the texture
% @param height The height of the texture
% @param depth The depth of the texture, i.e. the number of 2d layers.

% r --> width
r = zeros(width, height, depth);
for i = 1:height
    for j = 1:depth
        
    r(:, i, j) = (0: 1/(width-1):1);
    end
end

% g --> height
g = zeros(width, height, depth);
for i = 1:width
    for j = 1:depth
        
    g(i, :, j) = (0: 1/(height-1):1);
    end
end

% b --> depth
b = zeros(width, height, depth);
for i = 1:width
    for j = 1:height
        
    b(i, j, :) = (0: 1/(depth-1):1);
    end
end

% compose
% concattenate images
image = zeros(height, width * depth, 3);
for i = 1:depth
    image(:, ((i-1) * width + 1):i*width, 1) = r(:, :, i);
    image(:, ((i-1) * width + 1):i*width, 2) = g(:, :, i);
    image(:, ((i-1) * width + 1):i*width, 3) = b(:, :, i);
end

imshow(image)

end

