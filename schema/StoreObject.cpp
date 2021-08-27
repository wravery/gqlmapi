// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

// WARNING! Do not edit this file manually, your changes will be overwritten.

#include "MAPIObjects.h"

#include "graphqlservice/introspection/Introspection.h"

#include <algorithm>
#include <functional>
#include <sstream>
#include <stdexcept>
#include <unordered_map>

using namespace std::literals;

namespace graphql::mapi {
namespace object {

Store::Store()
	: service::Object({
		"Store"
	}, {
		{ R"gql(id)gql"sv, [this](service::ResolverParams&& params) { return resolveId(std::move(params)); } },
		{ R"gql(name)gql"sv, [this](service::ResolverParams&& params) { return resolveName(std::move(params)); } },
		{ R"gql(columns)gql"sv, [this](service::ResolverParams&& params) { return resolveColumns(std::move(params)); } },
		{ R"gql(__typename)gql"sv, [this](service::ResolverParams&& params) { return resolve_typename(std::move(params)); } },
		{ R"gql(rootFolders)gql"sv, [this](service::ResolverParams&& params) { return resolveRootFolders(std::move(params)); } },
		{ R"gql(itemProperties)gql"sv, [this](service::ResolverParams&& params) { return resolveItemProperties(std::move(params)); } },
		{ R"gql(specialFolders)gql"sv, [this](service::ResolverParams&& params) { return resolveSpecialFolders(std::move(params)); } },
		{ R"gql(folderProperties)gql"sv, [this](service::ResolverParams&& params) { return resolveFolderProperties(std::move(params)); } }
	})
{
}

std::future<service::ResolverResult> Store::resolveId(service::ResolverParams&& params)
{
	std::unique_lock resolverLock(_resolverMutex);
	auto directives = std::move(params.fieldDirectives);
	auto result = getId(service::FieldParams(std::move(params), std::move(directives)));
	resolverLock.unlock();

	return service::ModifiedResult<response::IdType>::convert(std::move(result), std::move(params));
}

std::future<service::ResolverResult> Store::resolveName(service::ResolverParams&& params)
{
	std::unique_lock resolverLock(_resolverMutex);
	auto directives = std::move(params.fieldDirectives);
	auto result = getName(service::FieldParams(std::move(params), std::move(directives)));
	resolverLock.unlock();

	return service::ModifiedResult<response::StringType>::convert(std::move(result), std::move(params));
}

std::future<service::ResolverResult> Store::resolveColumns(service::ResolverParams&& params)
{
	std::unique_lock resolverLock(_resolverMutex);
	auto directives = std::move(params.fieldDirectives);
	auto result = getColumns(service::FieldParams(std::move(params), std::move(directives)));
	resolverLock.unlock();

	return service::ModifiedResult<Property>::convert<service::TypeModifier::List, service::TypeModifier::Nullable>(std::move(result), std::move(params));
}

std::future<service::ResolverResult> Store::resolveRootFolders(service::ResolverParams&& params)
{
	auto argIds = service::ModifiedArgument<response::IdType>::require<service::TypeModifier::Nullable, service::TypeModifier::List>("ids", params.arguments);
	std::unique_lock resolverLock(_resolverMutex);
	auto directives = std::move(params.fieldDirectives);
	auto result = getRootFolders(service::FieldParams(std::move(params), std::move(directives)), std::move(argIds));
	resolverLock.unlock();

	return service::ModifiedResult<Folder>::convert<service::TypeModifier::List>(std::move(result), std::move(params));
}

std::future<service::ResolverResult> Store::resolveSpecialFolders(service::ResolverParams&& params)
{
	auto argIds = service::ModifiedArgument<mapi::SpecialFolder>::require<service::TypeModifier::List>("ids", params.arguments);
	std::unique_lock resolverLock(_resolverMutex);
	auto directives = std::move(params.fieldDirectives);
	auto result = getSpecialFolders(service::FieldParams(std::move(params), std::move(directives)), std::move(argIds));
	resolverLock.unlock();

	return service::ModifiedResult<Folder>::convert<service::TypeModifier::List, service::TypeModifier::Nullable>(std::move(result), std::move(params));
}

std::future<service::ResolverResult> Store::resolveFolderProperties(service::ResolverParams&& params)
{
	auto argFolderId = service::ModifiedArgument<response::IdType>::require("folderId", params.arguments);
	auto argIds = service::ModifiedArgument<mapi::Column>::require<service::TypeModifier::Nullable, service::TypeModifier::List>("ids", params.arguments);
	std::unique_lock resolverLock(_resolverMutex);
	auto directives = std::move(params.fieldDirectives);
	auto result = getFolderProperties(service::FieldParams(std::move(params), std::move(directives)), std::move(argFolderId), std::move(argIds));
	resolverLock.unlock();

	return service::ModifiedResult<Property>::convert<service::TypeModifier::List, service::TypeModifier::Nullable>(std::move(result), std::move(params));
}

std::future<service::ResolverResult> Store::resolveItemProperties(service::ResolverParams&& params)
{
	auto argItemId = service::ModifiedArgument<response::IdType>::require("itemId", params.arguments);
	auto argIds = service::ModifiedArgument<mapi::Column>::require<service::TypeModifier::Nullable, service::TypeModifier::List>("ids", params.arguments);
	std::unique_lock resolverLock(_resolverMutex);
	auto directives = std::move(params.fieldDirectives);
	auto result = getItemProperties(service::FieldParams(std::move(params), std::move(directives)), std::move(argItemId), std::move(argIds));
	resolverLock.unlock();

	return service::ModifiedResult<Property>::convert<service::TypeModifier::List, service::TypeModifier::Nullable>(std::move(result), std::move(params));
}

std::future<service::ResolverResult> Store::resolve_typename(service::ResolverParams&& params)
{
	return service::ModifiedResult<response::StringType>::convert(response::StringType{ R"gql(Store)gql" }, std::move(params));
}

} // namespace object

void AddStoreDetails(std::shared_ptr<schema::ObjectType> typeStore, const std::shared_ptr<schema::Schema>& schema)
{
	typeStore->AddFields({
		schema::Field::Make(R"gql(id)gql"sv, R"md(ID of this store)md"sv, std::nullopt, schema->WrapType(introspection::TypeKind::NON_NULL, schema->LookupType("ID"))),
		schema::Field::Make(R"gql(name)gql"sv, R"md(Name of this store)md"sv, std::nullopt, schema->WrapType(introspection::TypeKind::NON_NULL, schema->LookupType("String"))),
		schema::Field::Make(R"gql(columns)gql"sv, R"md(Columns specified with `@columns(ids: ...)` on the `Query.stores` field)md"sv, std::nullopt, schema->WrapType(introspection::TypeKind::NON_NULL, schema->WrapType(introspection::TypeKind::LIST, schema->LookupType("Property")))),
		schema::Field::Make(R"gql(rootFolders)gql"sv, R"md(List of root folders in the store)md"sv, std::nullopt, schema->WrapType(introspection::TypeKind::NON_NULL, schema->WrapType(introspection::TypeKind::LIST, schema->WrapType(introspection::TypeKind::NON_NULL, schema->LookupType("Folder")))), {
			schema::InputValue::Make(R"gql(ids)gql"sv, R"md(Optional list of root folder IDs, return all root folders if `null`)md"sv, schema->WrapType(introspection::TypeKind::LIST, schema->WrapType(introspection::TypeKind::NON_NULL, schema->LookupType("ID"))), R"gql(null)gql"sv)
		}),
		schema::Field::Make(R"gql(specialFolders)gql"sv, R"md(List of special folders in the store, some of which may not exist)md"sv, std::nullopt, schema->WrapType(introspection::TypeKind::NON_NULL, schema->WrapType(introspection::TypeKind::LIST, schema->LookupType("Folder"))), {
			schema::InputValue::Make(R"gql(ids)gql"sv, R"md(List of special folder IDs)md"sv, schema->WrapType(introspection::TypeKind::NON_NULL, schema->WrapType(introspection::TypeKind::LIST, schema->WrapType(introspection::TypeKind::NON_NULL, schema->LookupType("SpecialFolder")))), R"gql()gql"sv)
		}),
		schema::Field::Make(R"gql(folderProperties)gql"sv, R"md(Open a single folder and read any or all of its properties.)md"sv, std::nullopt, schema->WrapType(introspection::TypeKind::NON_NULL, schema->WrapType(introspection::TypeKind::LIST, schema->LookupType("Property"))), {
			schema::InputValue::Make(R"gql(folderId)gql"sv, R"md(Folder ID)md"sv, schema->WrapType(introspection::TypeKind::NON_NULL, schema->LookupType("ID")), R"gql()gql"sv),
			schema::InputValue::Make(R"gql(ids)gql"sv, R"md(Optional list of property IDs, returns all properties if `null`)md"sv, schema->WrapType(introspection::TypeKind::LIST, schema->WrapType(introspection::TypeKind::NON_NULL, schema->LookupType("Column"))), R"gql()gql"sv)
		}),
		schema::Field::Make(R"gql(itemProperties)gql"sv, R"md(Open a single item and read any or all of its properties.)md"sv, std::nullopt, schema->WrapType(introspection::TypeKind::NON_NULL, schema->WrapType(introspection::TypeKind::LIST, schema->LookupType("Property"))), {
			schema::InputValue::Make(R"gql(itemId)gql"sv, R"md(Item ID)md"sv, schema->WrapType(introspection::TypeKind::NON_NULL, schema->LookupType("ID")), R"gql()gql"sv),
			schema::InputValue::Make(R"gql(ids)gql"sv, R"md(Optional list of property IDs, returns all properties if `null`)md"sv, schema->WrapType(introspection::TypeKind::LIST, schema->WrapType(introspection::TypeKind::NON_NULL, schema->LookupType("Column"))), R"gql()gql"sv)
		})
	});
}

} // namespace graphql::mapi
