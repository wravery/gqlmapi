// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

// WARNING! Do not edit this file manually, your changes will be overwritten.

#include "ItemAddedObject.h"
#include "ItemObject.h"

#include "graphqlservice/internal/Schema.h"

#include "graphqlservice/introspection/IntrospectionSchema.h"

#include <algorithm>
#include <functional>
#include <sstream>
#include <stdexcept>
#include <unordered_map>

using namespace std::literals;

namespace graphql::mapi {
namespace object {

ItemAdded::ItemAdded(std::unique_ptr<Concept>&& pimpl) noexcept
	: service::Object{ getTypeNames(), getResolvers() }
	, _pimpl { std::move(pimpl) }
{
}

service::TypeNames ItemAdded::getTypeNames() const noexcept
{
	return {
		R"gql(ItemChange)gql"sv,
		R"gql(ItemAdded)gql"sv
	};
}

service::ResolverMap ItemAdded::getResolvers() const noexcept
{
	return {
		{ R"gql(added)gql"sv, [this](service::ResolverParams&& params) { return resolveAdded(std::move(params)); } },
		{ R"gql(index)gql"sv, [this](service::ResolverParams&& params) { return resolveIndex(std::move(params)); } },
		{ R"gql(__typename)gql"sv, [this](service::ResolverParams&& params) { return resolve_typename(std::move(params)); } }
	};
}

void ItemAdded::beginSelectionSet(const service::SelectionSetParams& params) const
{
	_pimpl->beginSelectionSet(params);
}

void ItemAdded::endSelectionSet(const service::SelectionSetParams& params) const
{
	_pimpl->endSelectionSet(params);
}

service::AwaitableResolver ItemAdded::resolveIndex(service::ResolverParams&& params) const
{
	std::unique_lock resolverLock(_resolverMutex);
	auto directives = std::move(params.fieldDirectives);
	auto result = _pimpl->getIndex(service::FieldParams(service::SelectionSetParams{ params }, std::move(directives)));
	resolverLock.unlock();

	return service::ModifiedResult<int>::convert(std::move(result), std::move(params));
}

service::AwaitableResolver ItemAdded::resolveAdded(service::ResolverParams&& params) const
{
	std::unique_lock resolverLock(_resolverMutex);
	auto directives = std::move(params.fieldDirectives);
	auto result = _pimpl->getAdded(service::FieldParams(service::SelectionSetParams{ params }, std::move(directives)));
	resolverLock.unlock();

	return service::ModifiedResult<Item>::convert(std::move(result), std::move(params));
}

service::AwaitableResolver ItemAdded::resolve_typename(service::ResolverParams&& params) const
{
	return service::ModifiedResult<std::string>::convert(std::string{ R"gql(ItemAdded)gql" }, std::move(params));
}

} // namespace object

void AddItemAddedDetails(const std::shared_ptr<schema::ObjectType>& typeItemAdded, const std::shared_ptr<schema::Schema>& schema)
{
	typeItemAdded->AddFields({
		schema::Field::Make(R"gql(index)gql"sv, R"md(Index in the subscribed window)md"sv, std::nullopt, schema->WrapType(introspection::TypeKind::NON_NULL, schema->LookupType(R"gql(Int)gql"sv))),
		schema::Field::Make(R"gql(added)gql"sv, R"md(`Item` that was added)md"sv, std::nullopt, schema->WrapType(introspection::TypeKind::NON_NULL, schema->LookupType(R"gql(Item)gql"sv)))
	});
}

} // namespace graphql::mapi
