classdef lab2 < matlab.apps.AppBase

    % Properties that correspond to app components
    properties (Access = public)
        UIFigure     matlab.ui.Figure
        GridLayout   matlab.ui.container.GridLayout
        LeftPanel    matlab.ui.container.Panel
        Button_2     matlab.ui.control.Button
        Slider       matlab.ui.control.Slider
        Label_3      matlab.ui.control.Label
        ButtonGroup  matlab.ui.container.ButtonGroup
        RGBButton    matlab.ui.control.RadioButton
        Button       matlab.ui.control.RadioButton
        SVDLabel     matlab.ui.control.Label
        RightPanel   matlab.ui.container.Panel
        Label_2      matlab.ui.control.Label
        Label        matlab.ui.control.Label
        Image_2      matlab.ui.control.Image
        Image        matlab.ui.control.Image
    end

    % Properties that correspond to apps with auto-reflow
    properties (Access = private)
        onePanelWidth = 576;
    end

    % Callbacks that handle component events
    methods (Access = private)

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
            app.ButtonGroup.Title = '图像类型';
            app.ButtonGroup.Position = [48 321 123 72];

            % Create Button
            app.Button = uiradiobutton(app.ButtonGroup);
            app.Button.Text = '灰度图';
            app.Button.Position = [11 26 58 22];
            app.Button.Value = true;

            % Create RGBButton
            app.RGBButton = uiradiobutton(app.ButtonGroup);
            app.RGBButton.Text = 'RGB彩色图';
            app.RGBButton.Position = [11 4 84 22];

            % Create Label_3
            app.Label_3 = uilabel(app.LeftPanel);
            app.Label_3.HorizontalAlignment = 'right';
            app.Label_3.Position = [11 252 65 44];
            app.Label_3.Text = '奇异值个数';

            % Create Slider
            app.Slider = uislider(app.LeftPanel);
            app.Slider.Position = [97 283 100 3];

            % Create Button_2
            app.Button_2 = uibutton(app.LeftPanel, 'push');
            app.Button_2.Position = [60 204 100 22];
            app.Button_2.Text = '确定';

            % Create RightPanel
            app.RightPanel = uipanel(app.GridLayout);
            app.RightPanel.Layout.Row = 1;
            app.RightPanel.Layout.Column = 2;

            % Create Image
            app.Image = uiimage(app.RightPanel);
            app.Image.Position = [36 261 272 192];

            % Create Image_2
            app.Image_2 = uiimage(app.RightPanel);
            app.Image_2.Position = [36 23 272 192];

            % Create Label
            app.Label = uilabel(app.RightPanel);
            app.Label.FontSize = 14;
            app.Label.Position = [345 346 33 22];
            app.Label.Text = '原图';

            % Create Label_2
            app.Label_2 = uilabel(app.RightPanel);
            app.Label_2.FontSize = 14;
            app.Label_2.Position = [338 108 47 22];
            app.Label_2.Text = '压缩后';

            % Show the figure after all components are created
            app.UIFigure.Visible = 'on';
        end
    end

    % App creation and deletion
    methods (Access = public)

        % Construct app
        function app = lab2

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