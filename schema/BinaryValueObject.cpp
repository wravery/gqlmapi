// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

// WARNING! Do not edit this file manually, your changes will be overwritten.

#include "BinaryValueObject.h"

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

BinaryValue::BinaryValue(std::unique_ptr<Concept>&& pimpl) noexcept
	: service::Object{ getTypeNames(), getResolvers() }
	, _pimpl { std::move(pimpl) }
{
}

service::TypeNames BinaryValue::getTypeNames() const noexcept
{
	return {
		R"gql(PropValue)gql"sv,
		R"gql(BinaryValue)gql"sv
	};
}

service::ResolverMap BinaryValue::getResolvers() const noexcept
{
	return {
		{ R"gql(value)gql"sv, [this](service::ResolverParams&& params) { return resolveValue(std::move(params)); } },
		{ R"gql(__typename)gql"sv, [this](service::ResolverParams&& params) { return resolve_typename(std::move(params)); } }
	};
}

void BinaryValue::beginSelectionSet(const service::SelectionSetParams& params) const
{
	_pimpl->beginSelectionSet(params);
}

void BinaryValue::endSelectionSet(const service::SelectionSetParams& params) const
{
	_pimpl->endSelectionSet(params);
}

service::AwaitableResolver BinaryValue::resolveValue(service::ResolverParams&& params) const
{
	std::unique_lock resolverLock(_resolverMutex);
	auto directives = std::move(params.fieldDirectives);
	auto result = _pimpl->getValue(service::FieldParams(service::SelectionSetParams{ params }, std::move(directives)));
	resolverLock.unlock();

	return service::ModifiedResult<response::IdType>::convert(std::move(result), std::move(params));
}

service::AwaitableResolver BinaryValue::resolve_typename(service::ResolverParams&& params) const
{
	return service::ModifiedResult<std::string>::convert(std::string{ R"gql(BinaryValue)gql" }, std::move(params));
}

} // namespace object

void AddBinaryValueDetails(const std::shared_ptr<schema::ObjectType>& typeBinaryValue, const std::shared_ptr<schema::Schema>& schema)
{
	typeBinaryValue->AddFields({
		schema::Field::Make(R"gql(value)gql"sv, R"md()md"sv, std::nullopt, schema->WrapType(introspection::TypeKind::NON_NULL, schema->LookupType(R"gql(ID)gql"sv)))
	});
}

} // namespace graphql::mapi
