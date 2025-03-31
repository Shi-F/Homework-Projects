classdef SVD_Compress < matlab.apps.AppBase

    % Properties that correspond to app components
    properties (Access = public)
        UIFigure         matlab.ui.Figure
        GridLayout       matlab.ui.container.GridLayout
        LeftPanel        matlab.ui.container.Panel
        SaveButton       matlab.ui.control.Button
        TextArea         matlab.ui.control.TextArea
        Label_4          matlab.ui.control.Label
        Button           matlab.ui.control.Button
        EditField        matlab.ui.control.NumericEditField
        Label_3          matlab.ui.control.Label
        ConfirmButton    matlab.ui.control.Button
        ButtonGroup      matlab.ui.container.ButtonGroup
        RGBButton        matlab.ui.control.RadioButton
        GrayButton       matlab.ui.control.RadioButton
        SVDLabel         matlab.ui.control.Label
        RightPanel       matlab.ui.container.Panel
        CompressedImage  matlab.ui.control.UIAxes
        SourceImage      matlab.ui.control.UIAxes
    end

    % Properties that correspond to apps with auto-reflow
    properties (Access = private)
        onePanelWidth = 576;
    end

    
    properties (Access = private)
        mode;
        rank;
        img;
        recentimg;
    end
    

    % Callbacks that handle component events
    methods (Access = private)

        % Selection changed function: ButtonGroup
        function ButtonGroupSelectionChanged(app, event)
            selectedButton = app.ButtonGroup.SelectedObject;
            app.mode = selectedButton.Text;
        end

        % Button pushed function: ConfirmButton
        function ConfirmButtonPushed(app, event)
            switch app.ButtonGroup.SelectedObject.Text
                case'灰度图'
                    imgray = rgb2gray(app.img);
                    imshow(imgray,'Parent',app.SourceImage);
                    [U, S, V] = svds(double(imgray), app.rank);
                    nim = uint8(U * S * V');
                    imshow(nim,'Parent',app.CompressedImage);
                    app.recentimg = nim;
                case'RGB彩色图'
                    imshow(app.img,'Parent',app.SourceImage);
                    nim = app.img;
                    for k = 1:3
                        R = app.img(:,:,k);
                        [U, S, V] = svds(double(R), app.rank);
                        nim(:,:,k) = uint8(U * S * V');
                    end
                    imshow(nim,'Parent',app.CompressedImage);
                    app.recentimg = nim;
            end
        end

        % Value changed function: EditField
        function EditFieldValueChanged(app, event)
            value = app.EditField.Value;
            app.rank = value;
        end

        % Button pushed function: Button
        function ButtonPushed(app, event)
            % 选择图像文件
            [filename, pathname] = uigetfile({'*.jpg;*.png;*.bmp','图像文件'}, '选择图像');
            if isequal(filename, 0)
                return; % 用户取消选择
            end
            img_path = fullfile(pathname, filename);
            app.TextArea.Value = img_path;
            app.img = imread(img_path);
            if size(app.img,3) == 1
                %读入灰度图像时转为RGB
                app.img = cat(3,app.img,app.img,app.img);
            end
            imshow(app.img,'Parent',app.SourceImage);
        end

        % Button pushed function: SaveButton
        function SaveButtonPushed(app, event)
            [filename, pathname] = uiputfile({'*.jpg;*.png;*.bmp','图像文件'}, '选择图像');
            if isequal(filename, 0)
                return; % 用户取消选择
            end
            img_path = fullfile(pathname, filename);
            imwrite(app.recentimg,img_path);
        end

        % Changes arrangement of the app based on UIFigure width
        function updateAppLayout(app, event)
            currentFigureWidth = app.UIFigure.Position(3);
            if(currentFigureWidth <= app.onePanelWidth)
                % Change to a 2x1 grid
                app.GridLayout.RowHeight = {480, 480};
                app.GridLayout.ColumnWidth = {'1x'};
                app.RightPanel.Layout.Row = 2;
                app.RightPanel.Layout.Column = 1;
            else
                % Change to a 1x2 grid
                app.GridLayout.RowHeight = {'1x'};
                app.GridLayout.ColumnWidth = {220, '1x'};
                app.RightPanel.Layout.Row = 1;
                app.RightPanel.Layout.Column = 2;
            end
        end
    end

    % Component initialization
    methods (Access = private)

        % Create UIFigure and components
        function createComponents(app)

            % Create UIFigure and hide until all components are created
            app.UIFigure = uifigure('Visible', 'off');
            app.UIFigure.AutoResizeChildren = 'off';
            app.UIFigure.Position = [100 100 640 480];
            app.UIFigure.Name = 'MATLAB App';
            app.UIFigure.SizeChangedFcn = createCallbackFcn(app, @updateAppLayout, true);

            % Create GridLayout
            app.GridLayout = uigridlayout(app.UIFigure);
            app.GridLayout.ColumnWidth = {220, '1x'};
            app.GridLayout.RowHeight = {'1x'};
            app.GridLayout.ColumnSpacing = 0;
            app.GridLayout.RowSpacing = 0;
            app.GridLayout.Padding = [0 0 0 0];
            app.GridLayout.Scrollable = 'on';

            % Create LeftPanel
            app.LeftPanel = uipanel(app.GridLayout);
            app.LeftPanel.Layout.Row = 1;
            app.LeftPanel.Layout.Column = 1;

            % Create SVDLabel
            app.SVDLabel = uilabel(app.LeftPanel);
            app.SVDLabel.FontSize = 18;
            app.SVDLabel.Position = [26 417 168 23];
            app.SVDLabel.Text = '基于SVD的图像压缩';

            % Create ButtonGroup
            app.ButtonGroup = uibuttongroup(app.LeftPanel);
            app.ButtonGroup.SelectionChangedFcn = createCallbackFcn(app, @ButtonGroupSelectionChanged, true);
            app.ButtonGroup.Title = '图像类型';
            app.ButtonGroup.Position = [48 204 123 72];

            % Create GrayButton
            app.GrayButton = uiradiobutton(app.ButtonGroup);
            app.GrayButton.Text = '灰度图';
            app.GrayButton.Position = [11 26 58 22];
            app.GrayButton.Value = true;

            % Create RGBButton
            app.RGBButton = uiradiobutton(app.ButtonGroup);
            app.RGBButton.Text = 'RGB彩色图';
            app.RGBButton.Position = [11 4 84 22];

            % Create ConfirmButton
            app.ConfirmButton = uibutton(app.LeftPanel, 'push');
            app.ConfirmButton.ButtonPushedFcn = createCallbackFcn(app, @ConfirmButtonPushed, true);
            app.ConfirmButton.Position = [60 106 100 22];
            app.ConfirmButton.Text = '压缩';

            % Create Label_3
            app.Label_3 = uilabel(app.LeftPanel);
            app.Label_3.HorizontalAlignment = 'right';
            app.Label_3.Position = [50 160 65 22];
            app.Label_3.Text = '奇异值个数';

            % Create EditField
            app.EditField = uieditfield(app.LeftPanel, 'numeric');
            app.EditField.ValueChangedFcn = createCallbackFcn(app, @EditFieldValueChanged, true);
            app.EditField.Position = [124 160 46 22];

            % Create Button
            app.Button = uibutton(app.LeftPanel, 'push');
            app.Button.ButtonPushedFcn = createCallbackFcn(app, @ButtonPushed, true);
            app.Button.Position = [60 380 100 22];
            app.Button.Text = '选择文件';

            % Create Label_4
            app.Label_4 = uilabel(app.LeftPanel);
            app.Label_4.HorizontalAlignment = 'right';
            app.Label_4.Position = [17 335 61 22];
            app.Label_4.Text = '当前文件：';

            % Create TextArea
            app.TextArea = uitextarea(app.LeftPanel);
            app.TextArea.Position = [77 337 125 20];

            % Create SaveButton
            app.SaveButton = uibutton(app.LeftPanel, 'push');
            app.SaveButton.ButtonPushedFcn = createCallbackFcn(app, @SaveButtonPushed, true);
            app.SaveButton.Position = [60 73 100 22];
            app.SaveButton.Text = '保存压缩文件';

            % Create RightPanel
            app.RightPanel = uipanel(app.GridLayout);
            app.RightPanel.Layout.Row = 1;
            app.RightPanel.Layout.Column = 2;

            % Create SourceImage
            app.SourceImage = uiaxes(app.RightPanel);
            title(app.SourceImage, '原图')
            app.SourceImage.XTick = [];
            app.SourceImage.YTick = [];
            app.SourceImage.TitleFontWeight = 'bold';
            app.SourceImage.Position = [19 252 379 206];

            % Create CompressedImage
            app.CompressedImage = uiaxes(app.RightPanel);
            title(app.CompressedImage, '压缩后')
            app.CompressedImage.XTick = [];
            app.CompressedImage.YTick = [];
            app.CompressedImage.TitleFontWeight = 'bold';
            app.CompressedImage.Position = [19 25 379 206];

            % Show the figure after all components are created
            app.UIFigure.Visible = 'on';
        end
    end

    % App creation and deletion
    methods (Access = public)

        % Construct app
        function app = SVD_Compress

            % Create UIFigure and components
            createComponents(app)

            % Register the app with App Designer
            registerApp(app, app.UIFigure)

            if nargout == 0
                clear app
            end
        end

        % Code that executes before app deletion
        function delete(app)

            % Delete UIFigure when app is deleted
            delete(app.UIFigure)
        end
    end
end