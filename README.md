# CarRacing
### develop with unreal engine 4

#### note:  
~	showdebug vehicle  

代码无提示，可通过点击编辑器的 文件->刷新visual studio项目->打开visual studio  解决。  

AVehicle : public AWheeledVehicle 会导致继承AVehicle的蓝图，重启UE4编辑器后继承AWheeledVehicle  
AMyVehicle : public AWheeledVehicle 蓝图继承AMyVehicle不会丢失继承关系。  

一般使用development editor进行开发，可使用Engine\Extras\UnrealVS\VS2015\UnrealVS.vsix插件直接在visual studio中编译。  
断点调试时使用debuggame editor。  
