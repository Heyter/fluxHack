hook.Add("Paint", "TEST", function()
	iDraw:StartDrawing_ViewRender() -- needed when draw on screen
		surface.SetDrawColor(255,255,255,255)
		surface.DrawText("������ ���!", 2, 2, 'ExampleFont')
		surface.DrawText("Hello World!", 10, 10, 'SchemeFont')
	iDraw:FinishDrawing_ViewRender()
end)