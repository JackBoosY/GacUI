/***********************************************************************
Vczh Library++ 3.0
Developer: Zihan Chen(vczh)
GacUI::Element System and Infrastructure Interfaces

Interfaces:
***********************************************************************/

#ifndef VCZH_PRESENTATION_ELEMENTS_GUIGRAPHICSRESOURCEMANAGER
#define VCZH_PRESENTATION_ELEMENTS_GUIGRAPHICSRESOURCEMANAGER

#include "GuiGraphicsElementInterfaces.h"
#include "GuiGraphicsDocumentInterfaces.h"
#include "../NativeWindow/GuiNativeWindow.h"

namespace vl
{
	namespace presentation
	{
		namespace elements
		{

/***********************************************************************
Resource Manager
***********************************************************************/

			/// <summary>
			/// This is a class for managing grpahics element factories and graphics renderer factories
			/// </summary>
			class GuiGraphicsResourceManager : public Object
			{
			protected:
				collections::List<WString>								elementTypes;
				collections::Array<Ptr<IGuiGraphicsRendererFactory>>	rendererFactories;
			public:
				/// <summary>
				/// Create a graphics resource manager without any predefined factories
				/// </summary>
				GuiGraphicsResourceManager();
				~GuiGraphicsResourceManager();

				/// <summary>
				/// Register a element type name.
				/// This function crashes when an element type has already been registered.
				/// </summary>
				/// <param name="elementTypeName">The element type.</param>
				/// <returns>A number identifies this element type.</returns>
				vint									RegisterElementType(const WString& elementTypeName);
				/// <summary>
				/// Register a <see cref="IGuiGraphicsRendererFactory"/> and bind it to an registered element type from <see cref="RegisterElementType"/>.
				/// This function crashes when an element type has already been binded a renderer factory.
				/// </summary>
				/// <param name="elementType">The element type to represent a graphics element factory.</param>
				/// <param name="factory">The instance of the graphics renderer factory to register.</param>
				void									RegisterRendererFactory(vint elementType, Ptr<IGuiGraphicsRendererFactory> factory);
				/// <summary>
				/// Get the instance of a registered <see cref="IGuiGraphicsRendererFactory"/> that is binded to a specified element type.
				/// </summary>
				/// <returns>Returns the renderer factory.</returns>
				/// <param name="elementType">The registered element type from <see cref="RegisterElementType"/> to get a binded graphics renderer factory.</param>
				IGuiGraphicsRendererFactory*			GetRendererFactory(vint elementType);
				/// <summary>
				/// Get the instance of a <see cref="IGuiGraphicsRenderTarget"/> that is binded to an <see cref="INativeWindow"/>.
				/// </summary>
				/// <param name="window">The specified window.</param>
				/// <returns>Returns the render target.</returns>
				virtual IGuiGraphicsRenderTarget*		GetRenderTarget(INativeWindow* window) = 0;
				/// <summary>
				/// Recreate the render target for the specified window.
				/// </summary>
				/// <param name="window">The specified window.</param>
				virtual void							RecreateRenderTarget(INativeWindow* window) = 0;
				/// <summary>
				/// Resize the render target to fit the current window size.
				/// </summary>
				/// <param name="window">The specified window.</param>
				virtual void							ResizeRenderTarget(INativeWindow* window) = 0;
				/// <summary>
				/// Get the renderer awared rich text document layout engine provider object.
				/// </summary>
				/// <returns>Returns the layout provider.</returns>
				virtual IGuiGraphicsLayoutProvider*		GetLayoutProvider() = 0;
			};

			/// <summary>
			/// Get the current <see cref="GuiGraphicsResourceManager"/>.
			/// </summary>
			/// <returns>Returns the current resource manager.</returns>
			extern GuiGraphicsResourceManager*			GetGuiGraphicsResourceManager();
			/// <summary>
			/// Set the current <see cref="GuiGraphicsResourceManager"/>.
			/// </summary>
			/// <param name="resourceManager">The resource manager to set.</param>
			extern void									SetGuiGraphicsResourceManager(GuiGraphicsResourceManager* resourceManager);

/***********************************************************************
Helpers
***********************************************************************/

			template<typename TElement>
			class GuiElementBase : public Object, public IGuiGraphicsElement, public Description<TElement>
			{
			protected:
				Ptr<IGuiGraphicsRenderer>				renderer;
				compositions::GuiGraphicsComposition*	ownerComposition = nullptr;

				void SetOwnerComposition(compositions::GuiGraphicsComposition* composition)override
				{
					ownerComposition = composition;
				}

				void InvokeOnCompositionStateChanged()
				{
					if (ownerComposition)
					{
						compositions::InvokeOnCompositionStateChanged(ownerComposition);
					}
				}

				void InvokeOnElementStateChanged()
				{
					if (renderer)
					{
						renderer->OnElementStateChanged();
					}
					InvokeOnCompositionStateChanged();
				}
			public:
				static TElement* Create()
				{
					auto rendererFactory = GetGuiGraphicsResourceManager()->GetRendererFactory(TElement::GetElementType());
					CHECK_ERROR(rendererFactory != nullptr, L"This element is not supported by the selected renderer.");

					auto element = new TElement;
					element->renderer = rendererFactory->Create();
					element->renderer->Initialize(element);
					return element;
				}

				~GuiElementBase()
				{
					if (renderer)
					{
						renderer->Finalize();
					}
				}

				IGuiGraphicsRenderer* GetRenderer()override
				{
					return renderer.Obj();
				}

				compositions::GuiGraphicsComposition* GetOwnerComposition()override
				{
					return ownerComposition;
				}
			};

#define DEFINE_GUI_GRAPHICS_ELEMENT(TELEMENT, ELEMENT_TYPE_NAME)\
				friend class GuiElementBase<TELEMENT>;\
			public:\
				static vint GetElementType()\
				{\
					static vint elementType = -1;\
					if (elementType == -1)\
					{\
						auto manager = GetGuiGraphicsResourceManager();\
						CHECK_ERROR(manager != nullptr, L"SetGuiGraphicsResourceManager must be called before registering element types.");\
						elementType = manager->RegisterElementType(WString::Unmanaged(ELEMENT_TYPE_NAME));\
					}\
					return elementType;\
				}\

#define DEFINE_GUI_GRAPHICS_RENDERER(TELEMENT, TRENDERER, TTARGET)\
			public:\
				class Factory : public Object, public IGuiGraphicsRendererFactory\
				{\
				public:\
					IGuiGraphicsRenderer* Create()\
					{\
						TRENDERER* renderer=new TRENDERER;\
						renderer->factory=this;\
						renderer->element=0;\
						renderer->renderTarget=0;\
						return renderer;\
					}\
				};\
			protected:\
				IGuiGraphicsRendererFactory*	factory;\
				TELEMENT*						element;\
				TTARGET*						renderTarget;\
				Size							minSize;\
			public:\
				static void Register()\
				{\
					auto manager = GetGuiGraphicsResourceManager();\
					CHECK_ERROR(manager != nullptr, L"SetGuiGraphicsResourceManager must be called before registering element renderers.");\
					manager->RegisterRendererFactory(TELEMENT::GetElementType(), new TRENDERER::Factory);\
				}\
				IGuiGraphicsRendererFactory* GetFactory()override\
				{\
					return factory;\
				}\
				void Initialize(IGuiGraphicsElement* _element)override\
				{\
					element=dynamic_cast<TELEMENT*>(_element);\
					InitializeInternal();\
				}\
				void Finalize()override\
				{\
					FinalizeInternal();\
				}\
				void SetRenderTarget(IGuiGraphicsRenderTarget* _renderTarget)override\
				{\
					TTARGET* oldRenderTarget=renderTarget;\
					renderTarget=dynamic_cast<TTARGET*>(_renderTarget);\
					RenderTargetChangedInternal(oldRenderTarget, renderTarget);\
				}\
				Size GetMinSize()override\
				{\
					return minSize;\
				}\

#define DEFINE_CACHED_RESOURCE_ALLOCATOR(TKEY, TVALUE)\
			public:\
				static const vint DeadPackageMax=32;\
				struct Package\
				{\
					TVALUE							resource;\
					vint								counter;\
					bool operator==(const Package& package)const{return false;}\
					bool operator!=(const Package& package)const{return true;}\
				};\
				struct DeadPackage\
				{\
					TKEY							key;\
					TVALUE							value;\
					bool operator==(const DeadPackage& package)const{return false;}\
					bool operator!=(const DeadPackage& package)const{return true;}\
				};\
				Dictionary<TKEY, Package>			aliveResources;\
				List<DeadPackage>					deadResources;\
			public:\
				TVALUE Create(const TKEY& key)\
				{\
					vint index=aliveResources.Keys().IndexOf(key);\
					if(index!=-1)\
					{\
						Package package=aliveResources.Values().Get(index);\
						package.counter++;\
						aliveResources.Set(key, package);\
						return package.resource;\
					}\
					TVALUE resource;\
					for(vint i=0;i<deadResources.Count();i++)\
					{\
						if(deadResources[i].key==key)\
						{\
							DeadPackage deadPackage=deadResources[i];\
							deadResources.RemoveAt(i);\
							resource=deadPackage.value;\
							break;\
						}\
					}\
					if(!resource)\
					{\
						resource=CreateInternal(key);\
					}\
					Package package;\
					package.resource=resource;\
					package.counter=1;\
					aliveResources.Add(key, package);\
					return package.resource;\
				}\
				void Destroy(const TKEY& key)\
				{\
					vint index=aliveResources.Keys().IndexOf(key);\
					if(index!=-1)\
					{\
						Package package=aliveResources.Values().Get(index);\
						package.counter--;\
						if(package.counter==0)\
						{\
							aliveResources.Remove(key);\
							if(deadResources.Count()==DeadPackageMax)\
							{\
								deadResources.RemoveAt(DeadPackageMax-1);\
							}\
							DeadPackage deadPackage;\
							deadPackage.key=key;\
							deadPackage.value=package.resource;\
							deadResources.Insert(0, deadPackage);\
						}\
						else\
						{\
							aliveResources.Set(key, package);\
						}\
					}\
				}
		}
	}
}

#endif