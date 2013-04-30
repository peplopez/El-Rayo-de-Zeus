-- Fichero Hud.lua

-- Vars initialization showed in HUD

numberAltarsActivated = 0
numberEnemiesInBase = 0
PM = 100
lifeBase = lifeBaseMax

--Funcion de inicializacion/carga de un layout y sus ventanas y subventanas
function initHud()
	if hud==nil then
		GUI.Windows:loadWindowLayout("Hud.layout")
		hud = GUI.Windows:getWindow("Hud")
		
		TitleEnemigosWindow = hud:getChild("Hud/TitleEnemigos")
		NumEnemigosWindow = hud:getChild("Hud/NumEnemigos")
		
		TitlePuntosMeritoWindow = hud:getChild("Hud/TitlePuntosMerito")
		PuntosMeritoWindow = hud:getChild("Hud/PuntosMerito")
		
		ImagenAltarWindow = hud:getChild("Hud/Altares")		
		NumberAltaresActivatedWindow = hud:getChild("Hud/NumAltares")
		
		PersonajeWindow = hud:getChild("Hud/Personaje")
		
		PlayernameWindow = hud:getChild("Hud/PlayerName")
		


		--Rayo1Window = hud:getChild("Hud/RayoBase1")
		--Rayo2Window = hud:getChild("Hud/RayoBase2")
		--Rayo3Window = hud:getChild("Hud/RayoBase3")

		RayoWindow = hud:getChild("Hud/RayoBase")
	end
end


--Funcion que muestra la ventana del HUD
function showHud()
	if hud ~= nil then
		GUI.System:setGUISheet(hud)
		hud:setVisible(true)
		hud:activate()
	end
end


--Funcion que oculta una ventana del GUI
function hideHud()
	if hud ~= nil then
		hud:deactivate()		
		hud:setVisible(false)
	end
end


function changeHudVisibility()
		if hud:isVisible() then
			hud:setVisible(false)
		else
			hud:setVisible(true)
		end
end


--Altars HUD functions
function incrementAltarsActivated()
  numberAltarsActivated = numberAltarsActivated+1
  --Control de rango
  if numberAltarsActivated > numberAltarsMax then
    numberAltarsActivated = numberAltarsMax
  end  
end

function decrementAltarsActivated()
  numberAltarsActivated = numberAltarsActivated-1
  --Control de rango  
  if numberAltarsActivated < 0 then
    numberAltarsActivated = 0
  end
end

function updateAltarsActivated()
   NumberAltaresActivatedWindow:setText(numberAltarsActivated..'/'..numberAltarsMax)
end

function showAltarsActivated(altarsactivated, altarsmax)
   NumberAltaresActivatedWindow:setText(altarsactivated..'/'..altarsmax)
end

--NumberEnemies HUD functions
function incrementEnemiesInBase()
  numberEnemiesInBase = numberEnemiesInBase+1
    --Control de rango  
	if numberEnemiesInBase > numberEnemiesInBaseMax then
		numberEnemiesInBase = numberEnemiesInBaseMax
	end  
end

function decrementEnemiesInBase()
  numberEnemiesInBase = numberEnemiesInBase-1
    --Control de rango
	if numberEnemiesInBase < 0 then
		numberEnemiesInBase = 0
	end
end

function updateEnemiesInBase()
   NumEnemigosWindow:setText(numberEnemiesInBase)
end

function showEnemiesInBase(enemies)
   NumEnemigosWindow:setText(enemies)
end

-- pM HUD functions
function incrementPM()
  PM = PM+incPM
end

function decrementPM()
  PM = PM-incPM
end

function updatePM()
   PuntosMeritoWindow:setText(PM)
end

function showPM(pm)
   PuntosMeritoWindow:setText(pm)
end

-- lifeBase HUD functions
function incrementBaseLife()
  lifeBase = lifeBase+1
  --Control de rango
	if lifeBase > lifeBaseMax then
		lifeBase = lifeBaseMax
	end	  
end

function decrementBaseLife()
  lifeBase = lifeBase-1
  --Control de rango
  if lifeBase < 0 then
	lifeBase = 0
  end  
end


function updateBaseLife()
	if lifeBase == 3 then
	   RayoWindow:setProperty("Image","set:HudBaseLife image:BaseLifeEntire")
	  --Rayo1Window:setProperty("Image","set:HudBackground image:RayoActivado")
	  --Rayo2Window:setProperty("Image","set:HudBackground image:RayoActivado")
	  --Rayo3Window:setProperty("Image","set:HudBackground image:RayoActivado")
    elseif lifeBase == 2 then
	    RayoWindow:setProperty("Image","set:HudBaseLife image:BaseLifeMedium")	
	  --Rayo1Window:setProperty("Image","set:HudBackground image:RayoActivado")
	  --Rayo2Window:setProperty("Image","set:HudBackground image:RayoActivado")
	  --Rayo3Window:setProperty("Image","set:HudBackground image:RayoDesactivado")
    elseif lifeBase == 1 then
	    RayoWindow:setProperty("Image","set:HudBaseLife image:BaseLifeMinimum")	
	  --Rayo1Window:setProperty("Image","set:HudBackground image:RayoActivado")
	  --Rayo2Window:setProperty("Image","set:HudBackground image:RayoDesactivado")
	  --Rayo3Window:setProperty("Image","set:HudBackground image:RayoDesactivado")
    elseif lifeBase == 0 then
	    RayoWindow:setProperty("Image","set:HudBaseLife image:BaseLifeMinimum")	
      --Rayo1Window:setProperty("Image","set:HudBackground image:RayoDesactivado")
	  --Rayo2Window:setProperty("Image","set:HudBackground image:RayoDesactivado")
	  --Rayo3Window:setProperty("Image","set:HudBackground image:RayoDesactivado")
    end
end

function showBaseLife()
	if lifeBase == 3 then
	    RayoWindow:setProperty("Image","set:HudBaseLife image:BaseLifeEntire")
	  --Rayo1Window:setProperty("Image","set:HudBackground image:RayoActivado")
	  --Rayo2Window:setProperty("Image","set:HudBackground image:RayoActivado")
	  --Rayo3Window:setProperty("Image","set:HudBackground image:RayoActivado")
    elseif lifeBase == 2 then
	    RayoWindow:setProperty("Image","set:HudBaseLife image:BaseLifeMedium")
	  --Rayo1Window:setProperty("Image","set:HudBackground image:RayoActivado")
	  --Rayo2Window:setProperty("Image","set:HudBackground image:RayoActivado")
	  --Rayo3Window:setProperty("Image","set:HudBackground image:RayoDesactivado")
    elseif lifeBase == 1 then
	    RayoWindow:setProperty("Image","set:HudBaseLife image:BaseLifeMinimum")
	  --Rayo1Window:setProperty("Image","set:HudBackground image:RayoActivado")
	  --Rayo2Window:setProperty("Image","set:HudBackground image:RayoDesactivado")
	  --Rayo3Window:setProperty("Image","set:HudBackground image:RayoDesactivado")
    elseif lifeBase == 0 then
	    RayoWindow:setProperty("Image","set:HudBaseLife image:BaseLifeMinimum")
      --Rayo1Window:setProperty("Image","set:HudBackground image:RayoDesactivado")
	  --Rayo2Window:setProperty("Image","set:HudBackground image:RayoDesactivado")
	  --Rayo3Window:setProperty("Image","set:HudBackground image:RayoDesactivado")
    end
end

function reloadHud()
		-- hud:setProperty("UnifiedAreaRect","{{0,0},{0,0},{0,504},{0,193}}")
		hud:setProperty("UnifiedAreaRect","{{0,0},{0,0},{0,257},{0,96}}")
		PersonajeWindow:setProperty("UnifiedAreaRect","{{0.05,0},{0.065,0},{0.375,0},{0.9,0}}")
		RayoWindow:setProperty("Image","set:HudBaseLife image:BaseLifeMedium")
		RayoWindow:setProperty("UnifiedAreaRect","{{0.82,0},{0.28,0},{0.92,0},{0.94,0}}")
		
		-- PlayernameWindow:setProperty("UnifiedAreaRect","{{0.5,0},{0.15,0},{0.9,0},{0.25,0}}")
		PlayernameWindow:setProperty("UnifiedAreaRect","{{0,100},{0.10,0},{0,250},{0.3,0}}")
		PlayernameWindow:setProperty("Font","Diogenes-14")
		
		--TitleEnemigosWindow:setProperty("UnifiedAreaRect","{{0.5,0},{0.28,0},{0.9,0},{0.48,0}}")
		TitleEnemigosWindow:setProperty("UnifiedAreaRect","{{0,110},{0.31,0},{0,180},{0.48,0}}")
		TitleEnemigosWindow:setProperty("Font","Diogenes-10")
		--NumEnemigosWindow:setProperty("UnifiedAreaRect","{{0.705,0},{0.28,0},{0.74,0},{0.48,0}}")
		NumEnemigosWindow:setProperty("UnifiedAreaRect","{{0,155},{0.31,0},{0,210},{0.48,0}}")
		
		--TitlePuntosMeritoWindow:setProperty("UnifiedAreaRect","{{0.4,0},{0.5,0},{0.55,0},{0.7,0}}")
		TitlePuntosMeritoWindow:setProperty("UnifiedAreaRect","{{0,110},{0.5,0},{0,150},{0.7,0}}")
		--PuntosMeritoWindow:setProperty("UnifiedAreaRect","{{0.56,0},{0.5,0},{0.68,0},{0.7,0}}")
		PuntosMeritoWindow:setProperty("UnifiedAreaRect","{{0,150},{0.5,0},{0,210},{0.7,0}}")
		
		ImagenAltarWindow:setProperty("UnifiedAreaRect","{{0,130},{0,68},{0,155},{0,96}}")
		NumberAltaresActivatedWindow:setProperty("UnifiedAreaRect","{{0,160},{0.7,0},{0,210},{1,0}}")
		NumberAltaresActivatedWindow:setProperty("Font","Diogenes-14")		
		
		--[[NumberEnemyWindow
		PuntosMeritoWindow
		NumberAltaresActivatedWindow
		Rayo1Window
		Rayo2Window
		Rayo3Window	]]
end

