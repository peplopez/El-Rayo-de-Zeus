-- Fichero Shop.lua

-- Vars initialization showed in SHOP

local area

--Funcion de inicializacion/carga de un layout y sus ventanas y subventanas
function initShop()

	if shop==nil then
	
		--GUI.Windows:loadWindowLayout("Shop.layout")
		--shop = GUI.Windows:getWindow("Shop")
		
		root = GUI.Windows:getWindow("Root")
		
		shop = GUI.Windows:getWindow("Root/Shop")
		
		area = shop:getInnerRectClipper() -- area es una variable de tipo Rect

		--tabControlWindow = shop:getChild("Shop/TabControl")
		
		--local tc = CEGUI.toTabControl(tabControlWindow)

        -- Add some pages to tab control
		--tc:addTab (GUI.Windows:loadWindowLayout("TabPageCriatures.layout"))	
        --tc:addTab (GUI.Windows:loadWindowLayout("TabPageItems.layout"))
			

	end
end


--Funcion que muestra la ventana del Shop
function showShop()
	--if shop ~= nil then
		--GUI.System:setGUISheet(shop)
		--shop:setVisible(true)
		--shop:activate()
	--end
	if root ~= nil then
		GUI.System:setGUISheet(root)
		shop:setVisible(true)
		shop:activate()
		GUI.Mouse:show()
	end	
end


--Funcion que oculta una ventana del GUI
function hideShop()
	--if shop ~= nil then
		--shop:deactivate()		
		--shop:setVisible(false)
	--end
    if root ~= nil then
		shop:deactivate()		
		shop:setVisible(false)
		GUI.Mouse:hide()
	end	
end


function changeShopVisibility()
		if shop:isVisible() then
			shop:setVisible(false)
			GUI.Mouse:hide()
		else
			shop:setVisible(true)
			GUI.Mouse:show()
			GUI.Mouse:setUnifiedConstraintArea(nil)
		end
end


function reloadShop()
	if shop ~= nil then
		shop:setProperty("UnifiedAreaRect","{{0.8,0},{0.1,0},{1,0},{0.9,0}}")
		
		--TitlePuntosMeritoWindow:setProperty("UnifiedAreaRect","{{0,105},{0.5,0},{0,150},{0.7,0}}")
		--PuntosMeritoWindow:setProperty("UnifiedAreaRect","{{0,150},{0.5,0},{0,210},{0.7,0}}")
	end
end

