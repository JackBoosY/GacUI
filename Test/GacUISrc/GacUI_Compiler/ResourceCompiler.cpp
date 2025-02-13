#include "ResourceCompiler.h"

using namespace vl::console;
using namespace vl::collections;
using namespace vl::stream;
using namespace vl::workflow::cppcodegen;

/***********************************************************************
DebugCallback
***********************************************************************/

void DebugCallback::OnLoadEnvironment()
{
	Console::WriteLine(L"    Workflow: Loading metadata from registered types ...");
}

void DebugCallback::OnInitialize(analyzer::WfLexicalScopeManager* manager)
{
	Console::WriteLine(L"    Workflow: Creating metadata from declarations ...");
}

void DebugCallback::OnValidateModule(Ptr<WfModule> module)
{
	Console::WriteLine((L"    Workflow: Validating module " + module->name.value + L" ..."));
}

void DebugCallback::OnGenerateMetadata()
{
	Console::WriteLine(L"    Workflow: Generating metadata ...");
}

void DebugCallback::OnGenerateCode(Ptr<WfModule> module)
{
	Console::WriteLine((L"    Workflow: Generating code for module " + module->name.value + L" ..."));
}

void DebugCallback::OnGenerateDebugInfo()
{
	Console::WriteLine(L"    Workflow: Generating debug information ...");
}

IWfCompilerCallback* DebugCallback::GetCompilerCallback()
{
	return this;
}

void DebugCallback::PrintPassName(vint passIndex)
{
	if (lastPassIndex != passIndex)
	{
		lastPassIndex = passIndex;

#define PRINT_PASS(PASS)\
		case IGuiResourceTypeResolver_Precompile::PASS:\
			Console::WriteLine((itow(passIndex + 1) + L"/" + itow(IGuiResourceTypeResolver_Precompile::Instance_Max + 1) + L": " L ## #PASS L""));\
			break;\

		switch (passIndex)
		{
			PRINT_PASS(Workflow_Collect)
			PRINT_PASS(Workflow_Compile)
			PRINT_PASS(Instance_CollectInstanceTypes)
			PRINT_PASS(Instance_CompileInstanceTypes)
			PRINT_PASS(Instance_CollectEventHandlers)
			PRINT_PASS(Instance_CompileEventHandlers)
			PRINT_PASS(Instance_GenerateInstanceClass)
			PRINT_PASS(Instance_CompileInstanceClass)
		}
	}
}

void DebugCallback::OnPerPass(vint passIndex)
{
	PrintPassName(passIndex);
}

void DebugCallback::OnPerResource(vint passIndex, Ptr<GuiResourceItem> resource)
{
	PrintPassName(passIndex);
	Console::WriteLine((L"    " + resource->GetResourcePath() + L""));
}

/***********************************************************************
CompileResources
***********************************************************************/

FilePath CompileResources(
	const WString& name,
	collections::List<WString>& dependencies,
	const WString& resourcePath,
	const WString& outputBinaryFolder,
	const WString& outputCppFolder,
	bool compressResource
)
{
	FilePath errorPath = outputBinaryFolder + name + L".UI.error.txt";
	FilePath workflowPath1 = outputBinaryFolder + name + L".Shared.UI.txt";
	FilePath workflowPath2 = outputBinaryFolder + name + L".TemporaryClass.UI.txt";
	FilePath workflowPath3 = outputBinaryFolder + name + L".InstanceClass.UI.txt";
	FilePath binaryPath = outputBinaryFolder + name + L".UI.bin";
	FilePath assemblyPath32 = outputBinaryFolder + name + L".UI.x86.bin";
	FilePath assemblyPath64 = outputBinaryFolder + name + L".UI.x64.bin";
#ifdef VCZH_64
	FilePath assemblyPath = assemblyPath64;
#else
	FilePath assemblyPath = assemblyPath32;
#endif

	List<GuiResourceError> errors;
	auto resource = GuiResource::LoadFromXml(resourcePath, errors);
	{
		auto metadata = resource->GetMetadata();
		metadata->name = name;
		CopyFrom(metadata->dependencies, dependencies);
	}
	DebugCallback debugCallback;
	File(errorPath).Delete();
	File(workflowPath1).Delete();
	File(workflowPath2).Delete();
	File(workflowPath3).Delete();
	File(binaryPath).Delete();
	File(assemblyPath32).Delete();
	File(assemblyPath64).Delete();

	auto precompiledFolder = PrecompileResource(resource, &debugCallback, errors);
	if (errors.Count() > 0)
	{
		WriteErrors(errors, errorPath);
		CHECK_FAIL(L"Error");
	}
	WriteWorkflowScript(precompiledFolder, L"Workflow/Shared", workflowPath1);
	WriteWorkflowScript(precompiledFolder, L"Workflow/TemporaryClass", workflowPath2);
	auto compiled = WriteWorkflowScript(precompiledFolder, L"Workflow/InstanceClass", workflowPath3);

	if (outputCppFolder != L"")
	{
		auto input = MakePtr<WfCppInput>(name);
		input->multiFile = WfCppFileSwitch::Enabled;
		input->reflection = WfCppFileSwitch::Enabled;
		input->comment = L"Source: Host.sln";
		input->normalIncludes.Add(L"../../../../Source/GacUI.h");
		input->reflectionIncludes.Add(L"../../../../Source/Reflection/TypeDescriptors/GuiReflectionPlugin.h");

		FilePath cppFolder = outputCppFolder;
		auto output = WriteCppCodesToFile(resource, compiled, input, cppFolder, errors);
		if (errors.Count() > 0)
		{
			WriteErrors(errors, errorPath);
			CHECK_FAIL(L"Error");
		}
		WriteEmbeddedResource(resource, input, output, compressResource, cppFolder / (name + L"Resource.cpp"));
	}

	WriteBinaryResource(resource, false, true, binaryPath, assemblyPath);
	return binaryPath;
}

void LoadResource(FilePath binaryPath)
{
	FileStream fileStream(binaryPath.GetFullPath(), FileStream::ReadOnly);
	GetResourceManager()->LoadResourceOrPending(fileStream, GuiResourceUsage::InstanceClass);
}