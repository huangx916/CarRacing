# CarRacing
### develop with unreal engine 4

### Note:  
~	showdebug vehicle  显示车辆信息  

代码无提示，可通过点击编辑器的 文件->刷新visual studio项目->打开visual studio  解决。  

`AVehicle : public AWheeledVehicle` 会导致继承`AVehicle`的蓝图，重启UE4编辑器后继承`AWheeledVehicle`  
`AMyVehicle : public AWheeledVehicle` 蓝图继承`AMyVehicle`不会丢失继承关系。  

一般使用development editor进行开发，可使用Engine\Extras\UnrealVS\VS2015\UnrealVS.vsix插件直接在visual studio中编译。  
断点调试时使用debuggame editor。  
