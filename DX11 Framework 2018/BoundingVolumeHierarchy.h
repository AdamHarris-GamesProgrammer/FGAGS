#pragma once
#include "Rigidbody.h"
#include "CollisionStructures.h"

template<class BoundingVolumeClass>
class BVHNode {
public:
	BVHNode* _children[2];

	BoundingVolumeClass _volume;

	Rigidbody* _body;

	BVHNode* _parent;

	BVHNode(BVHNode* parent, const BoundingVolumeClass& volume, Rigidbody* body = NULL)
		: _parent(parent), _volume(volume), _body(body) {
		_children[0] = _children[1] = NULL;
	}

	bool IsLeaf() const {
		//If the body is not null then this is a leaf 
		return (_body != NULL);
	}

	unsigned GetPotentialContacts(PotentialContact* contacts, unsigned limit) const {
		//If this is a leaf node then there is no contact
		if (IsLeaf() || limit == 0) return 0;

		return _children[0]->GetPotentialContacts(_children[1], contacts, limit);
	}

	void Insert(Rigidbody* body, const BoundingVolumeClass& volume) {
		if (IsLeaf()) {
			//Child one is a copy of us
			_children[0] = new BVHNode<BoundingVolumeClass>(this, _volume, _body);

			//Child 2 is the new body
			_children[1] = new BVHNode<BoundingVolumeClass>(this, volume, body);

			this->_body = NULL;

			RecalculateBoundingVolume();
		}
		else
		{
			if (_children[0]->_volume.GetGrowth(volume) < _children[1]->_volume.GetGrowth(volume)) {
				_children[0]->Insert(body, volume);
			}
			else {
				_children[1]->Insert(body, volume);
			}
		}
	}

	~BVHNode() {
		if (_parent) {
			BVHNode<BoundingVolumeClass>* sibling;
			if (_parent->_children[0] == this) sibling = _parent->_children[1];
			else sibling = _parent->_children[0];

			_parent->_volume = sibling->_volume;
			_parent->_body = sibling->_body;
			_parent->_children[0] = sibling->_children[0];
			_parent->_children[1] = sibling->_children[1];

			sibling->_parent = NULL;
			sibling->_body = NULL;
			sibling->_children[0] = NULL;
			sibling->_children[1] = NULL;
			delete sibling;

			_parent->RecalculateBoundingVolume();
		}

		if (_children[0]) {
			_children[0]->_parent = NULL;
			delete _children[0];
		}
		if (_children[1]) {
			_children->_parent = NULL;
			delete _children[1];
		}
	}

protected:
	bool Overlaps(const BVHNode<BoundingVolumeClass>* other) const {
		return _volume->Overlaps(other->_volume);
	}

	unsigned GetPotentialContactsWith(const BVHNode<BoundingVolumeClass>* other, PotentialContact* contacts, unsigned limit) const {
		if (!Overlaps(other) || limit == 0) return 0;

		//If both are leaf nodes then its a possible contact
		if (IsLeaf() && other->IsLeaf()) {
			contacts->_bodies[0] = body;
			contacts->_bodies[1] = other->_body;
			return 1;
		}

		if (other->IsLeaf() || (!IsLeaf() && _volume->GetSize() >= other->GetSize())) {
			unsigned count = _children[0]->GetPotentialContactsWith(other, contacts, limit);

			if (limit > count) {
				return count + _children[1]->GetPotentialContactsWith(other, contacts + count, limit - count);
			}
			else {
				return count;
			}
		}
		else
		{
			unsigned count = GetPotentialContactsWith(other->_children[0], contacts, limit);

			if (limit > count) {
				return count + GetPotentialContactsWith(other->_children[1], contacts + count, limit - count);
			}
			else {
				return count;
			}
		}
	}
};
