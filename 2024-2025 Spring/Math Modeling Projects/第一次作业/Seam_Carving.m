%   Copyright 漏 2025, Renjie Chen @ USTC



%% read image
im = imread('YexiLake.png');

%% draw 2 copies of the image
fig=figure('Units', 'pixel', 'Position', [100,100,1000,700], 'toolbar', 'none');
subplot(121); imshow(im); title({'Input image'});
subplot(122); himg = imshow(im*0); title({'Resized Image', 'Use the blue button to resize the input image'});
hToolResize = uipushtool('CData', reshape(repmat([0 0 1], 100, 1), [10 10 3]), 'TooltipString', 'apply seam carving method to resize image', ...
                        'ClickedCallback', @(~, ~) set(himg, 'cdata', seam_carve_image(im, size(im,1:2)-[0 100])));

%% TODO: implement function: searm_carve_image
% check the title above the image for how to use the user-interface to resize the input image
function im = seam_carve_image(im, sz)

tic;
% im = imresize(im, sz);

costfunction = @(im) sum( imfilter(im, [.5 1 .5; 1 -6 1; .5 1 .5]).^2, 3 );

k = size(im,2) - sz(2);
for i = 1:k
    G = costfunction(im);
    %% find a seam in G
    r = size(im,1);
    c = size(im,2);
    %% M shows the cost of each path
    M = zeros(r,c);
    M(1,:) = G(1,:);
    backtrack = zeros(r,c);
    for row = 2:r
        for col = 1:c
            if col == 1
                [M(row,col),backtrack(row,col)] = min(M(row-1,col:col+1)+[G(row,col),G(row,col)]);
            elseif col == c
                [M(row,col),backtrack(row,col)] = min(M(row-1,col-1:col)+[G(row,col),G(row,col)]);
                backtrack(row,col) = c-2+backtrack(row,col);
            else
                [M(row,col),backtrack(row,col)] = min(M(row-1,col-1:col+1)+[G(row,col),G(row,col),G(row,col)]);
                backtrack(row,col) = col-2+backtrack(row,col);
            end
        end
    end
    %% The minimum of the last row of M leads the seam
    %% remove seam from im
    [~,j] = min(M(r,:));
    for row = r:-1:1
        im(row,j:c-1,:) = im(row,j+1:c,:);
        j = backtrack(row,j);
    end
    im(:,c,:) = [];
end
    elapsedtime = toc;
    fprintf("程序运行时间为 %.2f 秒\n",elapsedtime);
end