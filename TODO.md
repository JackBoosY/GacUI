# TODO

## Issues

- Use the embedded data codegen / compress / decompress functions from `VlppParser2` to replace one in `GacUI`.

## OS Provider Features

- Drag and Drop framework.
- Windows
  - `INativeImage::SaveToStream` handle correctly for git format. It is possible that LoadFromStream need to process diff between git raw frames.

## Control Features

- `IColumnItemView`.
  - Change column size from `int` to `{minSize:int, columnOption:Absolute|Percentage, absolute:int, percentage:float}`
  - Column drag and drop.
    - ListView: only raises an event, developers need to update column headers and data by themselves.
    - DataGrid: swap column object, cells are changed due to binding.
- ListView.
  - `GroupedListView` and `BindableGroupedListView` from `GuiVirtualListView`: Group headers on all views.
  - `TreeListView` and `BindableTreeListView` from `GuiVirtualTreeView`.
- New `GuiListControl::IItemArranger` that can accept a `GuiRepeatCompositionBase`.
- Chart control.
- Code editor (need VlppParser2)
- Dock container.
- Touch support.

## Architecture

- New skin.
  - New default control templates with animation, written in XML generated C++ code.
  - A window can be called to update all its controls' and components' template.
- UI Automation.
- Test Automation on Tutorials.

## GacUI Resource Compiler

- `<eval Eval="expression"/>` tags.
- In the final pass, only workflow scripts are printed.
  - Use WorkflowCompiler.exe to do codegen externally.

## Graphics

- 2D drawing API.
- restriction-based MetaImageElement.
  - Remove PolygonElement.
  - Default non-text element renderer using 2D drawing API.
- Meta3DElement and Meta3D data structure.
  - Default Meta3DElement renderer using MetaImageElement with a surface sorting based algorithm.
- GIF player.
- video player.

## GacStudio

## Porting to New Platforms

- `INativeHostedController` + `INativeHostedWindow`.
  - Implementing these interfaces enable GacUI to run in the `hosted single window` mode, all GacUI windows and menus are rendered in one native window.
- SyncTree architecture that streams layout/element changes per `GuiControlHost`.
  - or called remoting whatever
- Port GacUI to other platforms:
  - Windows
    - Command/Powershell Line in Windows (hosted)
    - GDI (hosted mode)
    - Direct2d (hosted mode)
    - UWP (hosted mode + sync tree, optional)
  - Linux
    - Ncurses on Ubuntu (hosted)
    - gGac repo: complete development process for release
  - macOS
    - iGac repo: complete development process for release
  - Web Assembly (hosted mode + sync tree)
    - Canvas?
    - DOM?

## Binders for other Programming Languages

- XML still generate C++ files, and they need to be compiled to an appropriate form.
- Other programming languages doesn't access GacUI objects, instead they implement view model interfaces that defined in XML.
- Users need to compile C++ code by themselves.
- Users are not required to write C++ code, but they can.
- Users can add attributes in XML defined interfaces, so that GacGen can generate a list of them. After that, by reading generated workflow assembly files, glue code can be generated so that users can implement required view modelinterfaces directly. I will provide some tools for listed languages.
- Resources need to be generated in C++ files, because other languages cannot access GacUI objects, which means they cannot load resources manually.
- Languages:
  - [ ] JavaScript / TypeScript through Web assembly
  - [ ] .NET (core?) through dll
  - [ ] Python through dll
- Alternative option:
  - SyncViewMode technique (or call remoting whatever)
  - UI.exe connect view model
    - Compiled from generated code
    - Using dll or rpc to connect to a server written by other languages to serve view model objects.
    - Each way is provided from the library, end users only need a GuiMain function, offered from tutorials.

## Optional

### GacUI Resource Compiler

- Remove all loader implementation, enabling custom control types from developers.
  - Try not to include `GacUI.cpp` if `VCZH_DEBUG_METAONLY_REFLECTION` is on.
  - `mynamespaces::VirtualClasses_X` for adding virtual classes deriving from `mynamespaces::X`.
    - Containing methods like: `GuiSelectableButton* CheckBox()`.
  - `mynamespaces::InstanceLoader_X` for implementing different kinds of properties / constructors that do not exist in `mynamespaces::X`.
    - following a naming convention, e.g. `GuiTableComposition`'s rows/columns properties.
    - searching for correct default control templates.
- Incremental build inside single resource.
  -  Calculate dependencies by only parsing.
  -  Cache workflow assembly per resource in file.
  -  Codegen c++ from multiple workflow assembly.

### MISC

- Use collection interfaces on function signatures.
  - Only if `Vlpp` decides to add collection interfaces.
