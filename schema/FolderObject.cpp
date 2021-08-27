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

Folder::Folder()
	: service::Object({
		"Folder"
	}, {
		{ R"gql(id)gql"sv, [this](service::ResolverParams&& params) { return resolveId(std::move(params)); } },
		{ R"gql(name)gql"sv, [this](service::ResolverParams&& params) { return resolveName(std::move(params)); } },
		{ R"gql(count)gql"sv, [this](service::ResolverParams&& params) { return resolveCount(std::move(params)); } },
		{ R"gql(items)gql"sv, [this](service::ResolverParams&& params) { return resolveItems(std::move(params)); } },
		{ R"gql(store)gql"sv, [this](service::ResolverParams&& params) { return resolveStore(std::move(params)); } },
		{ R"gql(unread)gql"sv, [this](service::ResolverParams&& params) { return resolveUnread(std::move(params)); } },
		{ R"gql(columns)gql"sv, [this](service::ResolverParams&& params) { return resolveColumns(std::move(params)); } },
		{ R"gql(__typename)gql"sv, [this](service::ResolverParams&& params) { return resolve_typename(std::move(params)); } },
		{ R"gql(subFolders)gql"sv, [this](service::ResolverParams&& params) { return resolveSubFolders(std::move(params)); } },
		{ R"gql(parentFolder)gql"sv, [this](service::ResolverParams&& params) { return resolveParentFolder(std::move(params)); } },
		{ R"gql(conversations)gql"sv, [this](service::ResolverParams&& params) { return resolveConversations(std::move(params)); } },
		{ R"gql(specialFolder)gql"sv, [this](service::ResolverParams&& params) { return resolveSpecialFolder(std::move(params)); } }
	})
{
}

std::future<service::ResolverResult> Folder::resolveId(service::ResolverParams&& params)
{
	std::unique_lock resolverLock(_resolverMutex);
	auto directives = std::move(params.fieldDirectives);
	auto result = getId(service::FieldParams(std::move(params), std::move(directives)));
	resolverLock.unlock();

	return service::ModifiedResult<response::IdType>::convert(std::move(result), std::move(params));
}

std::future<service::ResolverResult> Folder::resolveParentFolder(service::ResolverParams&& params)
{
	std::unique_lock resolverLock(_resolverMutex);
	auto directives = std::move(params.fieldDirectives);
	auto result = getParentFolder(service::FieldParams(std::move(params), std::move(directives)));
	resolverLock.unlock();

	return service::ModifiedResult<Folder>::convert<service::TypeModifier::Nullable>(std::move(result), std::move(params));
}

std::future<service::ResolverResult> Folder::resolveStore(service::ResolverParams&& params)
{
	std::unique_lock resolverLock(_resolverMutex);
	auto directives = std::move(params.fieldDirectives);
	auto result = getStore(service::FieldParams(std::move(params), std::move(directives)));
	resolverLock.unlock();

	return service::ModifiedResult<Store>::convert(std::move(result), std::move(params));
}

std::future<service::ResolverResult> Folder::resolveName(service::ResolverParams&& params)
{
	std::unique_lock resolverLock(_resolverMutex);
	auto directives = std::move(params.fieldDirectives);
	auto result = getName(service::FieldParams(std::move(params), std::move(directives)));
	resolverLock.unlock();

	return service::ModifiedResult<response::StringType>::convert(std::move(result), std::move(params));
}

std::future<service::ResolverResult> Folder::resolveCount(service::ResolverParams&& params)
{
	std::unique_lock resolverLock(_resolverMutex);
	auto directives = std::move(params.fieldDirectives);
	auto result = getCount(service::FieldParams(std::move(params), std::move(directives)));
	resolverLock.unlock();

	return service::ModifiedResult<response::IntType>::convert(std::move(result), std::move(params));
}

std::future<service::ResolverResult> Folder::resolveUnread(service::ResolverParams&& params)
{
	std::unique_lock resolverLock(_resolverMutex);
	auto directives = std::move(params.fieldDirectives);
	auto result = getUnread(service::FieldParams(std::move(params), std::move(directives)));
	resolverLock.unlock();

	return service::ModifiedResult<response::IntType>::convert(std::move(result), std::move(params));
}

std::future<service::ResolverResult> Folder::resolveSpecialFolder(service::ResolverParams&& params)
{
	std::unique_lock resolverLock(_resolverMutex);
	auto directives = std::move(params.fieldDirectives);
	auto result = getSpecialFolder(service::FieldParams(std::move(params), std::move(directives)));
	resolverLock.unlock();

	return service::ModifiedResult<SpecialFolder>::convert<service::TypeModifier::Nullable>(std::move(result), std::move(params));
}

std::future<service::ResolverResult> Folder::resolveColumns(service::ResolverParams&& params)
{
	std::unique_lock resolverLock(_resolverMutex);
	auto directives = std::move(params.fieldDirectives);
	auto result = getColumns(service::FieldParams(std::move(params), std::move(directives)));
	resolverLock.unlock();

	return service::ModifiedResult<Property>::convert<service::TypeModifier::List, service::TypeModifier::Nullable>(std::move(result), std::move(params));
}

std::future<service::ResolverResult> Folder::resolveSubFolders(service::ResolverParams&& params)
{
	auto argIds = service::ModifiedArgument<response::IdType>::require<service::TypeModifier::Nullable, service::TypeModifier::List>("ids", params.arguments);
	std::unique_lock resolverLock(_resolverMutex);
	auto directives = std::move(params.fieldDirectives);
	auto result = getSubFolders(service::FieldParams(std::move(params), std::move(directives)), std::move(argIds));
	resolverLock.unlock();

	return service::ModifiedResult<Folder>::convert<service::TypeModifier::List>(std::move(result), std::move(params));
}

std::future<service::ResolverResult> Folder::resolveConversations(service::ResolverParams&& params)
{
	auto argIds = service::ModifiedArgument<response::IdType>::require<service::TypeModifier::Nullable, service::TypeModifier::List>("ids", params.arguments);
	std::unique_lock resolverLock(_resolverMutex);
	auto directives = std::move(params.fieldDirectives);
	auto result = getConversations(service::FieldParams(std::move(params), std::move(directives)), std::move(argIds));
	resolverLock.unlock();

	return service::ModifiedResult<Conversation>::convert<service::TypeModifier::List>(std::move(result), std::move(params));
}

std::future<service::ResolverResult> Folder::resolveItems(service::ResolverParams&& params)
{
	auto argIds = service::ModifiedArgument<response::IdType>::require<service::TypeModifier::Nullable, service::TypeModifier::List>("ids", params.arguments);
	std::unique_lock resolverLock(_resolverMutex);
	auto directives = std::move(params.fieldDirectives);
	auto result = getItems(service::FieldParams(std::move(params), std::move(directives)), std::move(argIds));
	resolverLock.unlock();

	return service::ModifiedResult<Item>::convert<service::TypeModifier::List>(std::move(result), std::move(params));
}

std::future<service::ResolverResult> Folder::resolve_typename(service::ResolverParams&& params)
{
	return service::ModifiedResult<response::StringType>::convert(response::StringType{ R"gql(Folder)gql" }, std::move(params));
}

} // namespace object

void AddFolderDetails(std::shared_ptr<schema::ObjectType> typeFolder, const std::shared_ptr<schema::Schema>& schema)
{
	typeFolder->AddFields({
		schema::Field::Make(R"gql(id)gql"sv, R"md(ID of this folder)md"sv, std::nullopt, schema->WrapType(introspection::TypeKind::NON_NULL, schema->LookupType("ID"))),
		schema::Field::Make(R"gql(parentFolder)gql"sv, R"md(Parent folder, if this is not a root folder in the store)md"sv, std::nullopt, schema->LookupType("Folder")),
		schema::Field::Make(R"gql(store)gql"sv, R"md(Store containing this folder)md"sv, std::nullopt, schema->WrapType(introspection::TypeKind::NON_NULL, schema->LookupType("Store"))),
		schema::Field::Make(R"gql(name)gql"sv, R"md(Name of this folder)md"sv, std::nullopt, schema->WrapType(introspection::TypeKind::NON_NULL, schema->LookupType("String"))),
		schema::Field::Make(R"gql(count)gql"sv, R"md(Total item count)md"sv, std::nullopt, schema->WrapType(introspection::TypeKind::NON_NULL, schema->LookupType("Int"))),
		schema::Field::Make(R"gql(unread)gql"sv, R"md(Unread item count)md"sv, std::nullopt, schema->WrapType(introspection::TypeKind::NON_NULL, schema->LookupType("Int"))),
		schema::Field::Make(R"gql(specialFolder)gql"sv, R"md(Special folder type, or `null` for normal folders)md"sv, std::nullopt, schema->LookupType("SpecialFolder")),
		schema::Field::Make(R"gql(columns)gql"sv, R"md(Columns specified with `@columns(ids: ...)` on the `Store.rootFolders` or `Folder.subFolders` field)md"sv, std::nullopt, schema->WrapType(introspection::TypeKind::NON_NULL, schema->WrapType(introspection::TypeKind::LIST, schema->LookupType("Property")))),
		schema::Field::Make(R"gql(subFolders)gql"sv, R"md(List of sub-folders under this folder in the hierarchy)md"sv, std::nullopt, schema->WrapType(introspection::TypeKind::NON_NULL, schema->WrapType(introspection::TypeKind::LIST, schema->WrapType(introspection::TypeKind::NON_NULL, schema->LookupType("Folder")))), {
			schema::InputValue::Make(R"gql(ids)gql"sv, R"md(Optional list of sub-folder IDs, return all immdediate sub-folders if `null`)md"sv, schema->WrapType(introspection::TypeKind::LIST, schema->WrapType(introspection::TypeKind::NON_NULL, schema->LookupType("ID"))), R"gql(null)gql"sv)
		}),
		schema::Field::Make(R"gql(conversations)gql"sv, R"md(List of items grouped into conversations in this folder)md"sv, std::nullopt, schema->WrapType(introspection::TypeKind::NON_NULL, schema->WrapType(introspection::TypeKind::LIST, schema->WrapType(introspection::TypeKind::NON_NULL, schema->LookupType("Conversation")))), {
			schema::InputValue::Make(R"gql(ids)gql"sv, R"md(Optional list of conversation IDs, return all conversation if `null`)md"sv, schema->WrapType(introspection::TypeKind::LIST, schema->WrapType(introspection::TypeKind::NON_NULL, schema->LookupType("ID"))), R"gql(null)gql"sv)
		}),
		schema::Field::Make(R"gql(items)gql"sv, R"md(List of items in this folder)md"sv, std::nullopt, schema->WrapType(introspection::TypeKind::NON_NULL, schema->WrapType(introspection::TypeKind::LIST, schema->WrapType(introspection::TypeKind::NON_NULL, schema->LookupType("Item")))), {
			schema::InputValue::Make(R"gql(ids)gql"sv, R"md(Optional list of item IDs, return all items if `null`)md"sv, schema->WrapType(introspection::TypeKind::LIST, schema->WrapType(introspection::TypeKind::NON_NULL, schema->LookupType("ID"))), R"gql(null)gql"sv)
		})
	});
}

} // namespace graphql::mapi
