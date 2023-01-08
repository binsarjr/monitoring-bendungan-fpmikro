<script>
	import moment from 'moment';
	import { dataLogicReady, isAman, isBahaya, isBencana, isSiaga } from '../stores/iot';
	import { dataCharts } from './../stores/preferences';
	import Chart from './Chart.svelte';
	const getNow = () => {
		const now = new Date();
		return `${now.getUTCFullYear()}-${
			now.getUTCMonth() + 1
		}-${now.getUTCDate()} ${now.getUTCHours()}:${now.getUTCMinutes()}:${now.getUTCSeconds()}`;
	};
	// onMount(() => {
	// 	const id = setInterval(() => {
	// 		$ketinggianAir = getRandomInt(500, 600);
	// 		$dataCharts[$dataCharts.length] = {
	// 			x: getNow(),
	// 			y: $ketinggianAir
	// 		};
	// 	}, 3000);
	// 	return () => {
	// 		clearInterval(id);
	// 	};
	// });
	const menitTerakhir = (decreaseIn = 1) => {
		const current = new Date();
		return `${current.getUTCFullYear()}-${
			current.getUTCMonth() + 1
		}-${current.getUTCDate()} ${current.getUTCHours()}:${
			current.getUTCMinutes() - decreaseIn
		}:${current.getUTCSeconds()}`;
	};
	let min = menitTerakhir(5);
	let max = getNow();
	$: if ($dataCharts) {
		min = menitTerakhir(5);
		max = getNow();
	}
</script>

{#if $dataLogicReady}
	<main class="w-full md:w-3/4">
		<Chart bind:min bind:max unit='hour' />
		<table class="min-w-full table-auto">
			<caption>
				Status: <span class="text-green-500 font-bold">Aman</span>,
				<span class="text-yellow-500 font-bold">Siaga</span>,
				<span class="text-red-500 font-bold">Bahaya</span>,
				<span class="text-black font-bold">Bencana</span>
			</caption>
			<thead>
				<tr class="bg-gray-600 text-white text-center">
					<th />
					<th class="p-2">Ketinggian Air</th>
					<th class="p-2">Periode Waktu</th>
				</tr>
			</thead>
			<tbody>
				{#each $dataCharts.reverse() as item, i}
					<tr class="font-semibold text-center hover:bg-gray-100">
						<td
							class:bg-green-500={isAman(item.y)}
							class:bg-yellow-500={isSiaga(item.y)}
							class:bg-red-500={isBahaya(item.y)}
							class:bg-black={isBencana(item.y)}
						/>
						<td class="p-2">{item.y}</td>
						<td class="p-2"
							>{moment.utc(item.x, 'YYYY-MM-DD HH:mm:ss').format('HH:mm:ss DD-MM-YYYY')}</td
						>
					</tr>
				{/each}
			</tbody>
		</table>
	</main>
{:else}
	<p>Mohon tunggu, sedang mengecek data</p>
{/if}
