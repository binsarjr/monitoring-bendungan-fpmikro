<script lang="ts">
	import {
		dataLogicReady,
		isAman,
		isBahaya,
		isBencana,
		isSiaga,
		ketinggianAir,
		maxAman,
		maxBahaya,
		maxSiaga,
		minBahaya,
		minBencana,
		minSiaga
	} from './../stores/iot';
	import { status } from '../stores/iot';
	import { browser } from '$app/environment';
	let kedip = false;

	let bencanaTimeoutId: any;
	$: if (browser) {
		if ($status === 'bencana') {
			bencanaTimeoutId && clearTimeout(bencanaTimeoutId);
			bencanaTimeoutId = setTimeout(() => {
				kedip = !kedip;
			}, 100);
		}
		let bahayaTimeoutId: any;
		if ($status === 'bahaya') {
			bahayaTimeoutId && clearTimeout(bahayaTimeoutId);
			bahayaTimeoutId = setTimeout(() => {
				kedip = !kedip;
			}, 500);
		}
		let siagaTimeoutId: any;
		if ($status === 'siaga') {
			siagaTimeoutId && clearTimeout(siagaTimeoutId);
			siagaTimeoutId = setTimeout(() => {
				kedip = !kedip;
			}, 1000);
		}

		if ($status !== 'Belum Terdeteksi') {
			if (isAman($ketinggianAir)) $status = 'aman';
			else if (isSiaga($ketinggianAir)) $status = 'siaga';
			else if (isBahaya($ketinggianAir)) $status = 'bahaya';
			else if (isBencana($ketinggianAir)) {
				$status = 'bencana';
			}

			if ($status == 'aman') kedip = false;
		}
	}
</script>

<div>
	<h3>Status Bendungan: {$status}</h3>
	{#if $dataLogicReady}
		{#if $status === 'bencana'}
			<div class="grid grid-cols-3 gap">
				{#each ['bg-green-500', 'bg-yellow-500', 'bg-red-500'] as color}
					<div class="mx-auto my-4 w-10 h-10 rounded-full {color}" class:opacity-50={kedip} />
				{/each}
			</div>
		{:else if $status === 'Belum Terdeteksi'}
			Loading...
		{:else}
			<div
				class="mx-auto my-4 w-10 h-10 rounded-full"
				class:bg-red-500={$status === 'bahaya'}
				class:bg-green-500={$status === 'aman'}
				class:bg-yellow-500={$status === 'siaga'}
				class:opacity-50={kedip && $status != 'aman'}
			/>
		{/if}
	{:else}
		Loading..
	{/if}
</div>
